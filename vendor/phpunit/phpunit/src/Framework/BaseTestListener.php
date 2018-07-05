<?hh

/*
 * This file is part of PHPUnit.
 *
 * (c) Sebastian Bergmann <sebastian@phpunit.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

use PHPUnit\Interfaces\TestListener;

use \Exception;

use PHPUnit\Exceptions\AssertionFailedError;

use \PHPUnit_Framework_Test;
use \PHPUnit_Framework_TestCase;

/**
 * An empty Listener that can be extended to implement TestListener
 * with just a few lines of code.
 *
 * @see TestListener for documentation on the API methods.
 * @since Class available since Release 4.0.0
 */
abstract class PHPUnit_Framework_BaseTestListener implements TestListener
{
    public function addError(PHPUnit_Framework_Test $test, Exception $e, float $time): void
    {
    }

    public function addWarning(PHPUnit_Framework_Test $test, PHPUnit_Framework_Warning $e, float $time): void
    {
    }

    public function addFailure(PHPUnit_Framework_Test $test, AssertionFailedError $e, float $time): void
    {
    }

    public function addIncompleteTest(PHPUnit_Framework_Test $test, Exception $e, float $time): void
    {
    }

    public function addRiskyTest(PHPUnit_Framework_Test $test, Exception $e, float $time): void
    {
    }

    public function addSkippedTest(PHPUnit_Framework_Test $test, Exception $e, float $time): void
    {
    }

    public function startTestSuite(PHPUnit_Framework_TestSuite $suite): void
    {
    }

    public function endTestSuite(PHPUnit_Framework_TestSuite $suite): void
    {
    }

    public function startTest(PHPUnit_Framework_Test $test): void
    {
    }

    public function endTest(PHPUnit_Framework_Test $test, float $time): void
    {
    }
}
