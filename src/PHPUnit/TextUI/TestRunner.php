<?hh 

/*
 * This file is part of PHPUnit.
 *
 * (c) Sebastian Bergmann <sebastian@phpunit.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */


use Zynga\Framework\ReflectionCache\V1\ReflectionClasses;

use SebastianBergmann\CodeCoverage\CodeCoverage;
use SebastianBergmann\CodeCoverage\Exception\CodeCoverageException as CodeCoverageException;
use SebastianBergmann\CodeCoverage\Filter as CodeCoverageFilter;
use SebastianBergmann\CodeCoverage\Report\Clover as CloverReport;
use SebastianBergmann\CodeCoverage\Report\Crap4j as Crap4jReport;
use SebastianBergmann\CodeCoverage\Report\Html\Facade as HtmlReport;
use SebastianBergmann\CodeCoverage\Report\PHP as PhpReport;
use SebastianBergmann\CodeCoverage\Report\Text as TextReport;
use SebastianBergmann\CodeCoverage\Report\Xml\Facade as XmlReport;
use SebastianBergmann\Environment\Runtime;

use Zynga\PHPUnit\V2\Filter\Container as FilterContainer;
use Zynga\PHPUnit\V2\Interfaces\TestInterface;
use Zynga\PHPUnit\V2\Interfaces\TestListenerInterface;
use Zynga\PHPUnit\V2\TestResult;
use Zynga\PHPUnit\V2\TestSuite;
use Zynga\PHPUnit\V2\Output\ResultPrinter;

/**
 * A TestRunner for the Command Line Interface (CLI)
 * PHP SAPI Module.
 *
 * @since Class available since Release 2.0.0
 */
class PHPUnit_TextUI_TestRunner extends PHPUnit_Runner_BaseTestRunner
{
    const SUCCESS_EXIT   = 0;
    const FAILURE_EXIT   = 1;
    const EXCEPTION_EXIT = 2;

    protected CodeCoverageFilter $codeCoverageFilter;
    protected CodeCoverage $codeCoverage;

    /**
     * @var PHPUnit_Runner_TestSuiteLoader
     */
    protected $loader = null;

    /**
     * @var ResultPrinter
     */
    protected $printer = null;

    /**
     * @var bool
     */
    protected static $versionStringPrinted = false;

    /**
     * @var Runtime
     */
    private $runtime;

    /**
     * @var bool
     */
    private $messagePrinted = false;

    /**
     * @param PHPUnit_Runner_TestSuiteLoader $loader
     * @param CodeCoverageFilter             $filter
     *
     * @since Method available since Release 3.4.0
     */
    public function __construct(?PHPUnit_Runner_TestSuiteLoader $loader = null, ?CodeCoverageFilter $filter = null)
    {
        if ($filter === null) {
            $filter = new CodeCoverageFilter();
        }

        $this->codeCoverageFilter = $filter;
        
        
        // JEO: If we ever need to support anything other than html reports,
        // i'll appologize now.
        $this->codeCoverage = new CodeCoverage(
         '/dev/null',
          null,
          $filter
        );

        $this->loader             = $loader;
        $this->runtime            = new Runtime();
    }

    /**
     * @param TestInterface|ReflectionClass $test
     * @param array                                  $arguments
     *
     * @return TestResult
     *
     * @throws PHPUnit_Framework_Exception
     */
    public static function run($test, array $arguments = [])
    {

        if ($test instanceof ReflectionClass) {
            $test = new TestSuite($test);
        }

        if ($test instanceof TestInterface) {
            $aTestRunner = new self();

            return $aTestRunner->doRun(
                $test,
                $arguments
            );
        } else {
            throw new PHPUnit_Framework_Exception(
                'No test case or test suite found.'
            );
        }
    }

    /**
     * @return TestResult
     */
    protected function createTestResult()
    {
        return new TestResult();
    }

    private function processSuiteFilters(TestInterface $suite, array $arguments)
    {
        if (!$arguments['filter'] &&
            empty($arguments['groups']) &&
            empty($arguments['excludeGroups'])) {
            return;
        }

        if (!empty($arguments['excludeGroups'])) {
          FilterContainer::addGroupExcludeFilter($arguments['excludeGroups']);
        }

        if (!empty($arguments['groups'])) {
          FilterContainer::addGroupIncludeFilter($arguments['groups']);
        }

        if ($arguments['filter']) {
          //echo "injectingTestFilter? " . $arguments['filter'] . "\n";
          FilterContainer::addTestFilter($arguments['filter']);
        }

    }

    /**
     * @param TestInterface $suite
     * @param array                  $arguments
     * @param bool                   $exit
     *
     * @return TestResult
     */
    public function doRun(TestInterface $suite, array $arguments = [], $exit = true)
    {
        if (isset($arguments['configuration'])) {
            $GLOBALS['__PHPUNIT_CONFIGURATION_FILE'] = $arguments['configuration'];
        }

        $this->handleConfiguration($arguments);

        $this->processSuiteFilters($suite, $arguments);

        if (isset($arguments['bootstrap'])) {
            $GLOBALS['__PHPUNIT_BOOTSTRAP'] = $arguments['bootstrap'];
        }

        if (is_int($arguments['repeat'])) {
            $test = new PHPUnit_Extensions_RepeatedTest(
                $suite,
                $arguments['repeat']
            );

            $suite = new TestSuite();
            $suite->addTest($test);
        }

        $result = $this->createTestResult();

        if (!$arguments['convertErrorsToExceptions']) {
            $result->convertErrorsToExceptions(false);
        }

        if (!$arguments['convertNoticesToExceptions']) {
            PHPUnit_Framework_Error_Notice::$enabled = false;
        }

        if (!$arguments['convertWarningsToExceptions']) {
            PHPUnit_Framework_Error_Warning::$enabled = false;
        }

        if ($arguments['stopOnError']) {
            $result->stopOnError(true);
        }

        if ($arguments['stopOnFailure']) {
            $result->stopOnFailure(true);
        }

        if ($arguments['stopOnWarning']) {
            $result->stopOnWarning(true);
        }

        if ($arguments['stopOnIncomplete']) {
            $result->stopOnIncomplete(true);
        }

        if ($arguments['stopOnRisky']) {
            $result->stopOnRisky(true);
        }

        if ($arguments['stopOnSkipped']) {
            $result->stopOnSkipped(true);
        }

        if ($this->printer === null) {
            if (isset($arguments['printer']) &&
                $arguments['printer'] instanceof PHPUnit_Util_Printer) {
                $this->printer = $arguments['printer'];
            } else {
                
                $this->printer = new ResultPrinter(
                    isset($arguments['stderr']) ? 'php://stderr' : null,
                    $arguments['verbose'],
                    $arguments['colors'],
                    $arguments['debug'],
                    $arguments['columns'],
                    $arguments['reverseList']
                );

            }
        }

        if (!$this->printer instanceof PHPUnit_Util_Log_TAP) {

            $this->writeMessage(
                'PHPUnit', PHPUnit_Runner_Version::getVersionString()
            );

            self::$versionStringPrinted = true;

            if ($arguments['verbose']) {
                $runtime = $this->runtime->getNameWithVersion();

                if ($this->runtime->hasXdebug()) {
                    $runtime .= sprintf(
                        ' with Xdebug %s',
                        phpversion('xdebug')
                    );
                }

                $this->writeMessage('Runtime', $runtime);

                if (isset($arguments['configuration'])) {
                    $this->writeMessage(
                        'Configuration',
                        $arguments['configuration']->getFilename()
                    );
                }
            }

            if (isset($arguments['deprecatedCheckForUnintentionallyCoveredCodeSettingUsed'])) {
                print "Warning:       Deprecated configuration setting \"checkForUnintentionallyCoveredCode\" used\n";
            }
        }

        foreach ($arguments['listeners'] as $listener) {
            $result->addListener($listener);
        }

        $result->addListener($this->printer);

        if (isset($arguments['testdoxHTMLFile'])) {
            $result->addListener(
                new PHPUnit_Util_TestDox_ResultPrinter_HTML(
                    $arguments['testdoxHTMLFile'],
                    $arguments['testdoxGroups'],
                    $arguments['testdoxExcludeGroups']
                )
            );
        }

        if (isset($arguments['testdoxTextFile'])) {
            $result->addListener(
                new PHPUnit_Util_TestDox_ResultPrinter_Text(
                    $arguments['testdoxTextFile'],
                    $arguments['testdoxGroups'],
                    $arguments['testdoxExcludeGroups']
                )
            );
        }

        if (isset($arguments['testdoxXMLFile'])) {
            $result->addListener(
                new PHPUnit_Util_TestDox_ResultPrinter_XML(
                    $arguments['testdoxXMLFile']
                )
            );
        }

        $codeCoverageReports = 0;

        if (isset($arguments['coverageClover'])) {
            $codeCoverageReports++;
        }

        if (isset($arguments['coverageCrap4J'])) {
            $codeCoverageReports++;
        }

        if (isset($arguments['coverageHtml'])) {
            $codeCoverageReports++;
        }

        if (isset($arguments['coveragePHP'])) {
            $codeCoverageReports++;
        }

        if (isset($arguments['coverageText'])) {
            $codeCoverageReports++;
        }

        if (isset($arguments['coverageXml'])) {
            $codeCoverageReports++;
        }

        if (isset($arguments['noCoverage'])) {
            $codeCoverageReports = 0;
        }

        if ($codeCoverageReports > 0) {
            if (!$this->runtime->canCollectCodeCoverage()) {
                $this->writeMessage('Error', 'No code coverage driver is available');

                $codeCoverageReports = 0;
            } elseif (!isset($arguments['whitelist']) && !$this->codeCoverageFilter->hasWhitelist()) {
                $this->writeMessage('Error', 'No whitelist configured, no code coverage will be generated');

                $codeCoverageReports = 0;
            }
        }

        if ($codeCoverageReports > 0) {

            $this->codeCoverage->setTarget('coverageHtml');

            $this->codeCoverage->setUnintentionallyCoveredSubclassesWhitelist(
                [SebastianBergmann\Comparator\Comparator::class]
            );

            $this->codeCoverage->setAddUncoveredFilesFromWhitelist(
                $arguments['addUncoveredFilesFromWhitelist']
            );

            $this->codeCoverage->setCheckForUnintentionallyCoveredCode(
                $arguments['strictCoverage']
            );

            $this->codeCoverage->setCheckForMissingCoversAnnotation(
                $arguments['strictCoverage']
            );

            $this->codeCoverage->setProcessUncoveredFilesFromWhitelist(
                $arguments['processUncoveredFilesFromWhitelist']
            );

            if (isset($arguments['forceCoversAnnotation'])) {
                $this->codeCoverage->setForceCoversAnnotation(
                    $arguments['forceCoversAnnotation']
                );
            }

            if (isset($arguments['disableCodeCoverageIgnore'])) {
                $this->codeCoverage->setDisableIgnoredLines(true);
            }

            if (isset($arguments['whitelist'])) {
                $this->codeCoverageFilter->addDirectoryToWhitelist($arguments['whitelist']);
            }

            $result->setCodeCoverage($this->codeCoverage);
        }

        if ($codeCoverageReports > 1) {
            if (isset($arguments['cacheTokens'])) {
                $this->codeCoverage->setCacheTokens($arguments['cacheTokens']);
            }
        }

        if (isset($arguments['jsonLogfile'])) {
            $result->addListener(
                new PHPUnit_Util_Log_JSON($arguments['jsonLogfile'])
            );
        }

        if (isset($arguments['tapLogfile'])) {
            $result->addListener(
                new PHPUnit_Util_Log_TAP($arguments['tapLogfile'])
            );
        }

        if (isset($arguments['teamcityLogfile'])) {
            $result->addListener(
                new PHPUnit_Util_Log_TeamCity($arguments['teamcityLogfile'])
            );
        }

        if (isset($arguments['junitLogfile'])) {
            $result->addListener(
                new PHPUnit_Util_Log_JUnit(
                    $arguments['junitLogfile'],
                    $arguments['reportUselessTests']
                )
            );
        }

        $result->beStrictAboutTestsThatDoNotTestAnything($arguments['reportUselessTests']);
        $result->beStrictAboutOutputDuringTests($arguments['disallowTestOutput']);
        $result->beStrictAboutTodoAnnotatedTests($arguments['disallowTodoAnnotatedTests']);
        $result->beStrictAboutResourceUsageDuringSmallTests($arguments['beStrictAboutResourceUsageDuringSmallTests']);
        $result->enforceTimeLimit($arguments['enforceTimeLimit']);
        $result->setTimeoutForSmallTests($arguments['timeoutForSmallTests']);
        $result->setTimeoutForMediumTests($arguments['timeoutForMediumTests']);
        $result->setTimeoutForLargeTests($arguments['timeoutForLargeTests']);

        $suite->run($result);

        unset($suite);
        $result->flushListeners();

        if ($this->printer instanceof ResultPrinter) {
            $this->printer->printResult($result);
        }

  
            if (isset($arguments['coverageClover'])) {
                $this->printer->write(
                    "\nGenerating code coverage report in Clover XML format ..."
                );

                try {
                    $writer = new CloverReport();
                    $writer->process($this->codeCoverage, $arguments['coverageClover']);

                    $this->printer->write(" done\n");
                    unset($writer);
                } catch (CodeCoverageException $e) {
                    $this->printer->write(
                        " failed\n" . $e->getMessage() . "\n"
                    );
                }
            }

            if (isset($arguments['coverageCrap4J'])) {
                $this->printer->write(
                    "\nGenerating Crap4J report XML file ..."
                );

                try {
                    $writer = new Crap4jReport($arguments['crap4jThreshold']);
                    $writer->process($this->codeCoverage, $arguments['coverageCrap4J']);

                    $this->printer->write(" done\n");
                    unset($writer);
                } catch (CodeCoverageException $e) {
                    $this->printer->write(
                        " failed\n" . $e->getMessage() . "\n"
                    );
                }
            }

            if (isset($arguments['coverageHtml'])) {
                
                $outputLocation = $arguments['coverageHtml'];

                $this->printer->write(
                    "\n" .
                    date('r') . " - CodeCoverage::HTML - start output=$outputLocation\n"
                );

                try {
                    $writer = new HtmlReport(
                        $arguments['reportLowUpperBound'],
                        $arguments['reportHighLowerBound'],
                        sprintf(
                            ' and <a href="https://phpunit.de/">PHPUnit %s</a>',
                            PHPUnit_Runner_Version::id()
                        )
                    );

                    $writer->process($this->codeCoverage, $arguments['coverageHtml']);

                    $this->printer->write(date('r') . " - CodeCoverage::HTML - done output=$outputLocation\n");
                    unset($writer);
                } catch (CodeCoverageException $e) {
                   
                    $this->printer->write(
                        " failed\n" . $e->getMessage() . "\n"
                    );
                }
            }

            if (isset($arguments['coveragePHP'])) {
                // JEO: I think we're removing code coverage in php layout
                // $this->printer->write(
                //     "\nGenerating code coverage report in PHP format ..."
                // );

                // try {
                //     $writer = new PhpReport();
                //     $writer->process($codeCoverage, $arguments['coveragePHP']);

                //     $this->printer->write(" done\n");
                //     unset($writer);
                // } catch (CodeCoverageException $e) {
                //     $this->printer->write(
                //         " failed\n" . $e->getMessage() . "\n"
                //     );
                // }
            }

            if (isset($arguments['coverageText'])) {
               
                 if ($arguments['coverageText'] == 'php://stdout') {
                     $outputStream = $this->printer;
                     $colors       = $arguments['colors'] && $arguments['colors'] != ResultPrinter::COLOR_NEVER;
                 } else {
                     $outputStream = new PHPUnit_Util_Printer($arguments['coverageText']);
                     $colors       = false;
                 }

                $processor = new TextReport(
                    $arguments['reportLowUpperBound'],
                    $arguments['reportHighLowerBound'],
                    $arguments['coverageTextShowUncoveredFiles'],
                    $arguments['coverageTextShowOnlySummary']
                );

                $outputStream->write(
                    $processor->process($this->codeCoverage, $colors)
                );

            }

            if (isset($arguments['coverageXml'])) {
                // JEO: I think we're removing code coverage in XML format.
                // $this->printer->write(
                //     "\nGenerating code coverage report in PHPUnit XML format ..."
                // );

                // try {
                //     $writer = new XmlReport;
                //     $writer->process($codeCoverage, $arguments['coverageXml']);

                //     $this->printer->write(" done\n");
                //     unset($writer);
                // } catch (CodeCoverageException $e) {
                //     $this->printer->write(
                //         " failed\n" . $e->getMessage() . "\n"
                //     );
                // }
            }
        

        if ($exit) {
            if ($result->wasSuccessful()) {
                if ($arguments['failOnRisky'] && !$result->allHarmless()) {
                    exit(self::FAILURE_EXIT);
                }

                if ($arguments['failOnWarning'] && $result->warningCount() > 0) {
                    exit(self::FAILURE_EXIT);
                }

                exit(self::SUCCESS_EXIT);
            }

            if ($result->errorCount() > 0) {
                exit(self::EXCEPTION_EXIT);
            }

            if ($result->failureCount() > 0) {
                exit(self::FAILURE_EXIT);
            }
        }

        return $result;
    }

    /**
     * @param ResultPrinter $resultPrinter
     */
    public function setPrinter(ResultPrinter $resultPrinter)
    {
        $this->printer = $resultPrinter;
    }

    /**
     * Override to define how to handle a failed loading of
     * a test suite.
     *
     * @param string $message
     */
    protected function runFailed($message)
    {
        $this->write($message . PHP_EOL);
        exit(self::FAILURE_EXIT);
    }

    /**
     * @param string $buffer
     *
     * @since Method available since Release 3.1.0
     */
    protected function write($buffer)
    {
        if (PHP_SAPI != 'cli' && PHP_SAPI != 'phpdbg') {
            $buffer = htmlspecialchars($buffer);
        }

        if ($this->printer !== null) {
            $this->printer->write($buffer);
        } else {
            print $buffer;
        }
    }

    /**
     * Returns the loader to be used.
     *
     * @return PHPUnit_Runner_TestSuiteLoader
     *
     * @since Method available since Release 2.2.0
     */
    public function getLoader()
    {
        if ($this->loader === null) {
            $this->loader = new PHPUnit_Runner_StandardTestSuiteLoader();
        }

        return $this->loader;
    }

    /**
     * @param array $arguments
     *
     * @since Method available since Release 3.2.1
     */
    protected function handleConfiguration(array &$arguments)
    {
        if (isset($arguments['configuration']) &&
            !$arguments['configuration'] instanceof PHPUnit_Util_Configuration) {
            $arguments['configuration'] = PHPUnit_Util_Configuration::getInstance(
                $arguments['configuration']
            );
        }

        $arguments['debug']     = isset($arguments['debug'])     ? $arguments['debug']     : false;
        $arguments['filter']    = isset($arguments['filter'])    ? $arguments['filter']    : false;
        $arguments['listeners'] = isset($arguments['listeners']) ? $arguments['listeners'] : [];

        if (isset($arguments['configuration'])) {
            $arguments['configuration']->handlePHPConfiguration();

            $phpunitConfiguration = $arguments['configuration']->getPHPUnitConfiguration();

            if (isset($phpunitConfiguration['deprecatedCheckForUnintentionallyCoveredCodeSettingUsed'])) {
                $arguments['deprecatedCheckForUnintentionallyCoveredCodeSettingUsed'] = true;
            }

            if (isset($phpunitConfiguration['bootstrap']) &&
                !isset($arguments['bootstrap'])) {
                $arguments['bootstrap'] = $phpunitConfiguration['bootstrap'];
            }

            if (isset($phpunitConfiguration['cacheTokens']) &&
                !isset($arguments['cacheTokens'])) {
                $arguments['cacheTokens'] = $phpunitConfiguration['cacheTokens'];
            }

            if (isset($phpunitConfiguration['colors']) &&
                !isset($arguments['colors'])) {
                $arguments['colors'] = $phpunitConfiguration['colors'];
            }

            if (isset($phpunitConfiguration['convertErrorsToExceptions']) &&
                !isset($arguments['convertErrorsToExceptions'])) {
                $arguments['convertErrorsToExceptions'] = $phpunitConfiguration['convertErrorsToExceptions'];
            }

            if (isset($phpunitConfiguration['convertNoticesToExceptions']) &&
                !isset($arguments['convertNoticesToExceptions'])) {
                $arguments['convertNoticesToExceptions'] = $phpunitConfiguration['convertNoticesToExceptions'];
            }

            if (isset($phpunitConfiguration['convertWarningsToExceptions']) &&
                !isset($arguments['convertWarningsToExceptions'])) {
                $arguments['convertWarningsToExceptions'] = $phpunitConfiguration['convertWarningsToExceptions'];
            }

            if (isset($phpunitConfiguration['stopOnError']) &&
                !isset($arguments['stopOnError'])) {
                $arguments['stopOnError'] = $phpunitConfiguration['stopOnError'];
            }

            if (isset($phpunitConfiguration['stopOnFailure']) &&
                !isset($arguments['stopOnFailure'])) {
                $arguments['stopOnFailure'] = $phpunitConfiguration['stopOnFailure'];
            }

            if (isset($phpunitConfiguration['stopOnWarning']) &&
                !isset($arguments['stopOnWarning'])) {
                $arguments['stopOnWarning'] = $phpunitConfiguration['stopOnWarning'];
            }

            if (isset($phpunitConfiguration['stopOnIncomplete']) &&
                !isset($arguments['stopOnIncomplete'])) {
                $arguments['stopOnIncomplete'] = $phpunitConfiguration['stopOnIncomplete'];
            }

            if (isset($phpunitConfiguration['stopOnRisky']) &&
                !isset($arguments['stopOnRisky'])) {
                $arguments['stopOnRisky'] = $phpunitConfiguration['stopOnRisky'];
            }

            if (isset($phpunitConfiguration['stopOnSkipped']) &&
                !isset($arguments['stopOnSkipped'])) {
                $arguments['stopOnSkipped'] = $phpunitConfiguration['stopOnSkipped'];
            }

            if (isset($phpunitConfiguration['failOnWarning']) &&
                !isset($arguments['failOnWarning'])) {
                $arguments['failOnWarning'] = $phpunitConfiguration['failOnWarning'];
            }

            if (isset($phpunitConfiguration['failOnRisky']) &&
                !isset($arguments['failOnRisky'])) {
                $arguments['failOnRisky'] = $phpunitConfiguration['failOnRisky'];
            }

            if (isset($phpunitConfiguration['timeoutForSmallTests']) &&
                !isset($arguments['timeoutForSmallTests'])) {
                $arguments['timeoutForSmallTests'] = $phpunitConfiguration['timeoutForSmallTests'];
            }

            if (isset($phpunitConfiguration['timeoutForMediumTests']) &&
                !isset($arguments['timeoutForMediumTests'])) {
                $arguments['timeoutForMediumTests'] = $phpunitConfiguration['timeoutForMediumTests'];
            }

            if (isset($phpunitConfiguration['timeoutForLargeTests']) &&
                !isset($arguments['timeoutForLargeTests'])) {
                $arguments['timeoutForLargeTests'] = $phpunitConfiguration['timeoutForLargeTests'];
            }

            if (isset($phpunitConfiguration['reportUselessTests']) &&
                !isset($arguments['reportUselessTests'])) {
                $arguments['reportUselessTests'] = $phpunitConfiguration['reportUselessTests'];
            }

            if (isset($phpunitConfiguration['strictCoverage']) &&
                !isset($arguments['strictCoverage'])) {
                $arguments['strictCoverage'] = $phpunitConfiguration['strictCoverage'];
            }

            if (isset($phpunitConfiguration['disallowTestOutput']) &&
                !isset($arguments['disallowTestOutput'])) {
                $arguments['disallowTestOutput'] = $phpunitConfiguration['disallowTestOutput'];
            }

            if (isset($phpunitConfiguration['enforceTimeLimit']) &&
                !isset($arguments['enforceTimeLimit'])) {
                $arguments['enforceTimeLimit'] = $phpunitConfiguration['enforceTimeLimit'];
            }

            if (isset($phpunitConfiguration['disallowTodoAnnotatedTests']) &&
                !isset($arguments['disallowTodoAnnotatedTests'])) {
                $arguments['disallowTodoAnnotatedTests'] = $phpunitConfiguration['disallowTodoAnnotatedTests'];
            }

            if (isset($phpunitConfiguration['beStrictAboutResourceUsageDuringSmallTests']) &&
                !isset($arguments['beStrictAboutResourceUsageDuringSmallTests'])) {
                $arguments['beStrictAboutResourceUsageDuringSmallTests'] = $phpunitConfiguration['beStrictAboutResourceUsageDuringSmallTests'];
            }

            if (isset($phpunitConfiguration['verbose']) &&
                !isset($arguments['verbose'])) {
                $arguments['verbose'] = $phpunitConfiguration['verbose'];
            }

            if (isset($phpunitConfiguration['reverseDefectList']) &&
                !isset($arguments['reverseList'])) {
                $arguments['reverseList'] = $phpunitConfiguration['reverseDefectList'];
            }

            if (isset($phpunitConfiguration['forceCoversAnnotation']) &&
                !isset($arguments['forceCoversAnnotation'])) {
                $arguments['forceCoversAnnotation'] = $phpunitConfiguration['forceCoversAnnotation'];
            }

            if (isset($phpunitConfiguration['disableCodeCoverageIgnore']) &&
                !isset($arguments['disableCodeCoverageIgnore'])) {
                $arguments['disableCodeCoverageIgnore'] = $phpunitConfiguration['disableCodeCoverageIgnore'];
            }

            if (isset($phpunitConfiguration['registerMockObjectsFromTestArgumentsRecursively']) &&
                !isset($arguments['registerMockObjectsFromTestArgumentsRecursively'])) {
                $arguments['registerMockObjectsFromTestArgumentsRecursively'] = $phpunitConfiguration['registerMockObjectsFromTestArgumentsRecursively'];
            }

            $groupCliArgs = [];

            if (!empty($arguments['groups'])) {
                $groupCliArgs = $arguments['groups'];
            }

            $groupConfiguration = $arguments['configuration']->getGroupConfiguration();

            if (!empty($groupConfiguration['include']) &&
                !isset($arguments['groups'])) {
                $arguments['groups'] = $groupConfiguration['include'];
            }

            if (!empty($groupConfiguration['exclude']) &&
                !isset($arguments['excludeGroups'])) {
                $arguments['excludeGroups'] = array_diff($groupConfiguration['exclude'], $groupCliArgs);
            }

            foreach ($arguments['configuration']->getListenerConfiguration() as $listener) {
                if (!class_exists($listener['class'], false) &&
                    $listener['file'] !== '') {
                    require_once $listener['file'];
                }

                if (class_exists($listener['class'])) {
                    if (count($listener['arguments']) == 0) {
                        $listener = new $listener['class']();
                    } else {
                        $listenerClass = ReflectionClasses::getReflection(
                            $listener['class']
                        );
                        if ( $listenerClass instanceof ReflectionClass ) {
                        $listener      = $listenerClass->newInstanceArgs(
                            $listener['arguments']
                        );
                        }
                    }

                    if ($listener instanceof TestListenerInterface) {
                        $arguments['listeners'][] = $listener;
                    }
                }
            }

            $loggingConfiguration = $arguments['configuration']->getLoggingConfiguration();

            if (isset($loggingConfiguration['coverage-clover']) &&
                !isset($arguments['coverageClover'])) {
                $arguments['coverageClover'] = $loggingConfiguration['coverage-clover'];
            }

            if (isset($loggingConfiguration['coverage-crap4j']) &&
                !isset($arguments['coverageCrap4J'])) {
                $arguments['coverageCrap4J'] = $loggingConfiguration['coverage-crap4j'];

                if (isset($loggingConfiguration['crap4jThreshold']) &&
                    !isset($arguments['crap4jThreshold'])) {
                    $arguments['crap4jThreshold'] = $loggingConfiguration['crap4jThreshold'];
                }
            }

            if (isset($loggingConfiguration['coverage-html']) &&
                !isset($arguments['coverageHtml'])) {
                if (isset($loggingConfiguration['lowUpperBound']) &&
                    !isset($arguments['reportLowUpperBound'])) {
                    $arguments['reportLowUpperBound'] = $loggingConfiguration['lowUpperBound'];
                }

                if (isset($loggingConfiguration['highLowerBound']) &&
                    !isset($arguments['reportHighLowerBound'])) {
                    $arguments['reportHighLowerBound'] = $loggingConfiguration['highLowerBound'];
                }

                $arguments['coverageHtml'] = $loggingConfiguration['coverage-html'];
            }

            if (isset($loggingConfiguration['coverage-php']) &&
                !isset($arguments['coveragePHP'])) {
                $arguments['coveragePHP'] = $loggingConfiguration['coverage-php'];
            }

            if (isset($loggingConfiguration['coverage-text']) &&
                !isset($arguments['coverageText'])) {
                $arguments['coverageText'] = $loggingConfiguration['coverage-text'];
                if (isset($loggingConfiguration['coverageTextShowUncoveredFiles'])) {
                    $arguments['coverageTextShowUncoveredFiles'] = $loggingConfiguration['coverageTextShowUncoveredFiles'];
                } else {
                    $arguments['coverageTextShowUncoveredFiles'] = false;
                }
                if (isset($loggingConfiguration['coverageTextShowOnlySummary'])) {
                    $arguments['coverageTextShowOnlySummary'] = $loggingConfiguration['coverageTextShowOnlySummary'];
                } else {
                    $arguments['coverageTextShowOnlySummary'] = false;
                }
            }

            if (isset($loggingConfiguration['coverage-xml']) &&
                !isset($arguments['coverageXml'])) {
                $arguments['coverageXml'] = $loggingConfiguration['coverage-xml'];
            }

            if (isset($loggingConfiguration['json']) &&
                !isset($arguments['jsonLogfile'])) {
                $arguments['jsonLogfile'] = $loggingConfiguration['json'];
            }

            if (isset($loggingConfiguration['plain'])) {
                $arguments['listeners'][] = new ResultPrinter(
                    $loggingConfiguration['plain'],
                    true
                );
            }

            if (isset($loggingConfiguration['tap']) &&
                !isset($arguments['tapLogfile'])) {
                $arguments['tapLogfile'] = $loggingConfiguration['tap'];
            }

            if (isset($loggingConfiguration['teamcity']) &&
                !isset($arguments['teamcityLogfile'])) {
                $arguments['teamcityLogfile'] = $loggingConfiguration['teamcity'];
            }

            if (isset($loggingConfiguration['junit']) &&
                !isset($arguments['junitLogfile'])) {
                $arguments['junitLogfile'] = $loggingConfiguration['junit'];
            }

            if (isset($loggingConfiguration['testdox-html']) &&
                !isset($arguments['testdoxHTMLFile'])) {
                $arguments['testdoxHTMLFile'] = $loggingConfiguration['testdox-html'];
            }

            if (isset($loggingConfiguration['testdox-text']) &&
                !isset($arguments['testdoxTextFile'])) {
                $arguments['testdoxTextFile'] = $loggingConfiguration['testdox-text'];
            }

            if (isset($loggingConfiguration['testdox-xml']) &&
                !isset($arguments['testdoxXMLFile'])) {
                $arguments['testdoxXMLFile'] = $loggingConfiguration['testdox-xml'];
            }

            if ((isset($arguments['coverageClover']) ||
                isset($arguments['coverageCrap4J']) ||
                isset($arguments['coverageHtml']) ||
                isset($arguments['coveragePHP']) ||
                isset($arguments['coverageText']) ||
                isset($arguments['coverageXml'])) &&
                $this->runtime->canCollectCodeCoverage()) {
                $filterConfiguration                             = $arguments['configuration']->getFilterConfiguration();
                $arguments['addUncoveredFilesFromWhitelist']     = $filterConfiguration['whitelist']['addUncoveredFilesFromWhitelist'];
                $arguments['processUncoveredFilesFromWhitelist'] = $filterConfiguration['whitelist']['processUncoveredFilesFromWhitelist'];

                foreach ($filterConfiguration['whitelist']['include']['directory'] as $dir) {
                    $this->codeCoverageFilter->addDirectoryToWhitelist(
                        $dir['path'],
                        $dir['suffix'],
                        $dir['prefix']
                    );
                }

                foreach ($filterConfiguration['whitelist']['include']['file'] as $file) {
                    $this->codeCoverageFilter->addFileToWhitelist($file);
                }

                foreach ($filterConfiguration['whitelist']['exclude']['directory'] as $dir) {
                    $this->codeCoverageFilter->removeDirectoryFromWhitelist(
                        $dir['path'],
                        $dir['suffix'],
                        $dir['prefix']
                    );
                }

                foreach ($filterConfiguration['whitelist']['exclude']['file'] as $file) {
                    $this->codeCoverageFilter->removeFileFromWhitelist($file);
                }
            }
        }

        $arguments['addUncoveredFilesFromWhitelist']                  = isset($arguments['addUncoveredFilesFromWhitelist'])                  ? $arguments['addUncoveredFilesFromWhitelist']                  : true;
        $arguments['processUncoveredFilesFromWhitelist']              = isset($arguments['processUncoveredFilesFromWhitelist'])              ? $arguments['processUncoveredFilesFromWhitelist']              : false;
        $arguments['cacheTokens']                                     = isset($arguments['cacheTokens'])                                     ? $arguments['cacheTokens']                                     : false;
        $arguments['columns']                                         = isset($arguments['columns'])                                         ? $arguments['columns']                                         : 80;
        $arguments['colors']                                          = isset($arguments['colors'])                                          ? $arguments['colors']                                          : ResultPrinter::COLOR_DEFAULT;
        $arguments['convertErrorsToExceptions']                       = isset($arguments['convertErrorsToExceptions'])                       ? $arguments['convertErrorsToExceptions']                       : true;
        $arguments['convertNoticesToExceptions']                      = isset($arguments['convertNoticesToExceptions'])                      ? $arguments['convertNoticesToExceptions']                      : true;
        $arguments['convertWarningsToExceptions']                     = isset($arguments['convertWarningsToExceptions'])                     ? $arguments['convertWarningsToExceptions']                     : true;
        $arguments['excludeGroups']                                   = isset($arguments['excludeGroups'])                                   ? $arguments['excludeGroups']                                   : [];
        $arguments['groups']                                          = isset($arguments['groups'])                                          ? $arguments['groups']                                          : [];
        $arguments['repeat']                                          = isset($arguments['repeat'])                                          ? $arguments['repeat']                                          : false;
        $arguments['reportHighLowerBound']                            = isset($arguments['reportHighLowerBound'])                            ? $arguments['reportHighLowerBound']                            : 90;
        $arguments['reportLowUpperBound']                             = isset($arguments['reportLowUpperBound'])                             ? $arguments['reportLowUpperBound']                             : 50;
        $arguments['crap4jThreshold']                                 = isset($arguments['crap4jThreshold'])                                 ? $arguments['crap4jThreshold']                                 : 30;
        $arguments['stopOnError']                                     = isset($arguments['stopOnError'])                                     ? $arguments['stopOnError']                                     : false;
        $arguments['stopOnFailure']                                   = isset($arguments['stopOnFailure'])                                   ? $arguments['stopOnFailure']                                   : false;
        $arguments['stopOnWarning']                                   = isset($arguments['stopOnWarning'])                                   ? $arguments['stopOnWarning']                                   : false;
        $arguments['stopOnIncomplete']                                = isset($arguments['stopOnIncomplete'])                                ? $arguments['stopOnIncomplete']                                : false;
        $arguments['stopOnRisky']                                     = isset($arguments['stopOnRisky'])                                     ? $arguments['stopOnRisky']                                     : false;
        $arguments['stopOnSkipped']                                   = isset($arguments['stopOnSkipped'])                                   ? $arguments['stopOnSkipped']                                   : false;
        $arguments['failOnWarning']                                   = isset($arguments['failOnWarning'])                                   ? $arguments['failOnWarning']                                   : false;
        $arguments['failOnRisky']                                     = isset($arguments['failOnRisky'])                                     ? $arguments['failOnRisky']                                     : false;
        $arguments['timeoutForSmallTests']                            = isset($arguments['timeoutForSmallTests'])                            ? $arguments['timeoutForSmallTests']                            : 1;
        $arguments['timeoutForMediumTests']                           = isset($arguments['timeoutForMediumTests'])                           ? $arguments['timeoutForMediumTests']                           : 10;
        $arguments['timeoutForLargeTests']                            = isset($arguments['timeoutForLargeTests'])                            ? $arguments['timeoutForLargeTests']                            : 60;
        $arguments['reportUselessTests']                              = isset($arguments['reportUselessTests'])                              ? $arguments['reportUselessTests']                              : false;
        $arguments['strictCoverage']                                  = isset($arguments['strictCoverage'])                                  ? $arguments['strictCoverage']                                  : false;
        $arguments['disallowTestOutput']                              = isset($arguments['disallowTestOutput'])                              ? $arguments['disallowTestOutput']                              : false;
        $arguments['enforceTimeLimit']                                = isset($arguments['enforceTimeLimit'])                                ? $arguments['enforceTimeLimit']                                : false;
        $arguments['disallowTodoAnnotatedTests']                      = isset($arguments['disallowTodoAnnotatedTests'])                      ? $arguments['disallowTodoAnnotatedTests']                      : false;
        $arguments['beStrictAboutResourceUsageDuringSmallTests']      = isset($arguments['beStrictAboutResourceUsageDuringSmallTests'])      ? $arguments['beStrictAboutResourceUsageDuringSmallTests']      : false;
        $arguments['reverseList']                                     = isset($arguments['reverseList'])                                     ? $arguments['reverseList']                                     : false;
        $arguments['registerMockObjectsFromTestArgumentsRecursively'] = isset($arguments['registerMockObjectsFromTestArgumentsRecursively']) ? $arguments['registerMockObjectsFromTestArgumentsRecursively'] : false;
        $arguments['verbose']                                         = isset($arguments['verbose'])                                         ? $arguments['verbose']                                         : false;
        $arguments['testdoxExcludeGroups']                            = isset($arguments['testdoxExcludeGroups'])                            ? $arguments['testdoxExcludeGroups']                            : [];
        $arguments['testdoxGroups']                                   = isset($arguments['testdoxGroups'])                                   ? $arguments['testdoxGroups']                                   : [];
    }

    /**
     * @param string $type
     * @param string $message
     *
     * @since Method available since Release 5.0.0
     */
    private function writeMessage($type, $message)
    {
        if (!$this->messagePrinted) {
            $this->write("\n");
        }

        $this->write(
            sprintf(
                "%s - %-15s%s\n",
                date('r'),
                $type . ':',
                $message
            )
        );

        $this->messagePrinted = true;
    }
}
