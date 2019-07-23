<?php

// autoload_static.php @generated by Composer

namespace Composer\Autoload;

class ComposerStaticInitd67eef4e653bb27f765e27b84e0b5425
{
    public static $prefixLengthsPsr4 = array (
        'p' =>
        array (
            'phpDocumentor\\Reflection\\' => 25,
        ),
        'W' =>
        array (
            'Webmozart\\Assert\\' => 17,
        ),
        'S' =>
        array (
            'Symfony\\Component\\Yaml\\' => 23,
        ),
        'D' =>
        array (
            'Doctrine\\Instantiator\\' => 22,
            'DeepCopy\\' => 9,
        ),
    );

    public static $prefixDirsPsr4 = array (
        'phpDocumentor\\Reflection\\' =>
        array (
            0 => __DIR__ . '/..' . '/phpdocumentor/reflection-common/src',
            1 => __DIR__ . '/..' . '/phpdocumentor/type-resolver/src',
            2 => __DIR__ . '/..' . '/phpdocumentor/reflection-docblock/src',
        ),
        'Webmozart\\Assert\\' =>
        array (
            0 => __DIR__ . '/..' . '/webmozart/assert/src',
        ),
        'Symfony\\Component\\Yaml\\' =>
        array (
            0 => __DIR__ . '/..' . '/symfony/yaml',
        ),
        'Doctrine\\Instantiator\\' =>
        array (
            0 => __DIR__ . '/..' . '/doctrine/instantiator/src/Doctrine/Instantiator',
        ),
        'DeepCopy\\' =>
        array (
            0 => __DIR__ . '/..' . '/myclabs/deep-copy/src/DeepCopy',
        ),
    );

    public static $prefixesPsr0 = array (
        'P' =>
        array (
            'Prophecy\\' =>
            array (
                0 => __DIR__ . '/..' . '/phpspec/prophecy/src',
            ),
        ),
        'J' =>
        array (
            'JohnKary' =>
            array (
                0 => __DIR__ . '/..' . '/johnkary/phpunit-speedtrap/src',
            ),
        ),
    );

    public static $classMap = array (
        'File_Iterator' => __DIR__ . '/..' . '/phpunit/php-file-iterator/src/Iterator.php',
        'File_Iterator_Facade' => __DIR__ . '/..' . '/phpunit/php-file-iterator/src/Facade.php',
        'File_Iterator_Factory' => __DIR__ . '/..' . '/phpunit/php-file-iterator/src/Factory.php',
        'PHPUnit\\Framework\\TestCase' => __DIR__ . '/..' . '/phpunit/phpunit/src/ForwardCompatibility/TestCase.php',
        'PHPUnit_Exception' => __DIR__ . '/..' . '/phpunit/phpunit/src/Exception.php',
        'PHPUnit_Extensions_GroupTestSuite' => __DIR__ . '/..' . '/phpunit/phpunit/src/Extensions/GroupTestSuite.php',
        'PHPUnit_Extensions_PhptTestCase' => __DIR__ . '/..' . '/phpunit/phpunit/src/Extensions/PhptTestCase.php',
        'PHPUnit_Extensions_PhptTestSuite' => __DIR__ . '/..' . '/phpunit/phpunit/src/Extensions/PhptTestSuite.php',
        'PHPUnit_Extensions_RepeatedTest' => __DIR__ . '/..' . '/phpunit/phpunit/src/Extensions/RepeatedTest.php',
        'PHPUnit_Extensions_TestDecorator' => __DIR__ . '/..' . '/phpunit/phpunit/src/Extensions/TestDecorator.php',
        'PHPUnit_Extensions_TicketListener' => __DIR__ . '/..' . '/phpunit/phpunit/src/Extensions/TicketListener.php',
        'PHPUnit_Framework_Assert' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/Assert.php',
        'PHPUnit_Framework_AssertionFailedError' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/AssertionFailedError.php',
        'PHPUnit_Framework_BaseTestListener' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/BaseTestListener.php',
        'PHPUnit_Framework_CodeCoverageException' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/CodeCoverageException.php',
        'PHPUnit_Framework_CoveredCodeNotExecutedException' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/PHPUnit_Framework_CoveredCodeNotExecutedException.php',
        'PHPUnit_Framework_Error' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/Error.php',
        'PHPUnit_Framework_Error_Deprecated' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/Error/Deprecated.php',
        'PHPUnit_Framework_Error_Notice' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/Error/Notice.php',
        'PHPUnit_Framework_Exception' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/Exception.php',
        'PHPUnit_Framework_ExpectationFailedException' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/ExpectationFailedException.php',
        'PHPUnit_Framework_IncompleteTestCase' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/IncompleteTestCase.php',
        'PHPUnit_Framework_InvalidCoversTargetException' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/InvalidCoversTargetException.php',
        'PHPUnit_Framework_MissingCoversAnnotationException' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/PHPUnit_Framework_MissingCoversAnnotationException.php',
        'PHPUnit_Framework_MockObject_BadMethodCallException' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Exception/BadMethodCallException.php',
        'PHPUnit_Framework_MockObject_Builder_Identity' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Builder/Identity.php',
        'PHPUnit_Framework_MockObject_Builder_InvocationMocker' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Builder/InvocationMocker.php',
        'PHPUnit_Framework_MockObject_Builder_Match' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Builder/Match.php',
        'PHPUnit_Framework_MockObject_Builder_MethodNameMatch' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Builder/MethodNameMatch.php',
        'PHPUnit_Framework_MockObject_Builder_Namespace' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Builder/Namespace.php',
        'PHPUnit_Framework_MockObject_Builder_ParametersMatch' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Builder/ParametersMatch.php',
        'PHPUnit_Framework_MockObject_Builder_Stub' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Builder/Stub.php',
        'PHPUnit_Framework_MockObject_Exception' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Exception/Exception.php',
        'PHPUnit_Framework_MockObject_Generator' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Generator.php',
        'PHPUnit_Framework_MockObject_Invocation' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Invocation.php',
        'PHPUnit_Framework_MockObject_InvocationMocker' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/InvocationMocker.php',
        'PHPUnit_Framework_MockObject_Invocation_Object' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Invocation/Object.php',
        'PHPUnit_Framework_MockObject_Invocation_Static' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Invocation/Static.php',
        'PHPUnit_Framework_MockObject_Invokable' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Invokable.php',
        'PHPUnit_Framework_MockObject_Matcher' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher.php',
        'PHPUnit_Framework_MockObject_Matcher_AnyInvokedCount' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/AnyInvokedCount.php',
        'PHPUnit_Framework_MockObject_Matcher_AnyParameters' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/AnyParameters.php',
        'PHPUnit_Framework_MockObject_Matcher_ConsecutiveParameters' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/ConsecutiveParameters.php',
        'PHPUnit_Framework_MockObject_Matcher_Invocation' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/Invocation.php',
        'PHPUnit_Framework_MockObject_Matcher_InvokedAtIndex' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/InvokedAtIndex.php',
        'PHPUnit_Framework_MockObject_Matcher_InvokedAtLeastCount' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/InvokedAtLeastCount.php',
        'PHPUnit_Framework_MockObject_Matcher_InvokedAtLeastOnce' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/InvokedAtLeastOnce.php',
        'PHPUnit_Framework_MockObject_Matcher_InvokedAtMostCount' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/InvokedAtMostCount.php',
        'PHPUnit_Framework_MockObject_Matcher_InvokedCount' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/InvokedCount.php',
        'PHPUnit_Framework_MockObject_Matcher_InvokedRecorder' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/InvokedRecorder.php',
        'PHPUnit_Framework_MockObject_Matcher_MethodName' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/MethodName.php',
        'PHPUnit_Framework_MockObject_Matcher_Parameters' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/Parameters.php',
        'PHPUnit_Framework_MockObject_Matcher_StatelessInvocation' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Matcher/StatelessInvocation.php',
        'PHPUnit_Framework_MockObject_MockBuilder' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/MockBuilder.php',
        'PHPUnit_Framework_MockObject_MockObject' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/MockObject.php',
        'PHPUnit_Framework_MockObject_RuntimeException' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Exception/RuntimeException.php',
        'PHPUnit_Framework_MockObject_Stub' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub.php',
        'PHPUnit_Framework_MockObject_Stub_ConsecutiveCalls' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/ConsecutiveCalls.php',
        'PHPUnit_Framework_MockObject_Stub_Exception' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/Exception.php',
        'PHPUnit_Framework_MockObject_Stub_MatcherCollection' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/MatcherCollection.php',
        'PHPUnit_Framework_MockObject_Stub_Return' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/Return.php',
        'PHPUnit_Framework_MockObject_Stub_ReturnArgument' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/ReturnArgument.php',
        'PHPUnit_Framework_MockObject_Stub_ReturnCallback' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/ReturnCallback.php',
        'PHPUnit_Framework_MockObject_Stub_ReturnReference' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/ReturnReference.php',
        'PHPUnit_Framework_MockObject_Stub_ReturnSelf' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/ReturnSelf.php',
        'PHPUnit_Framework_MockObject_Stub_ReturnValueMap' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Stub/ReturnValueMap.php',
        'PHPUnit_Framework_MockObject_Verifiable' => __DIR__ . '/..' . '/phpunit/phpunit-mock-objects/src/Framework/MockObject/Verifiable.php',
        'PHPUnit_Framework_OutputError' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/OutputError.php',
        'PHPUnit_Framework_RiskyTest' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/RiskyTest.php',
        'PHPUnit_Framework_SelfDescribing' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/SelfDescribing.php',
        'PHPUnit_Framework_SkippedTestCase' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/SkippedTestCase.php',
        'PHPUnit_Framework_SyntheticError' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/SyntheticError.php',
        'TestInterface' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/Test.php',
        'PHPUnit_Framework_TestCase' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/TestCase.php',
        'PHPUnit_Framework_TestListener' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/TestListener.php',
        'PHPUnit_Framework_UnintentionallyCoveredCodeError' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/UnintentionallyCoveredCodeError.php',
        'PHPUnit_Framework_Warning' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/Warning.php',
        'PHPUnit_Framework_WarningTestCase' => __DIR__ . '/..' . '/phpunit/phpunit/src/Framework/WarningTestCase.php',
        'PHPUnit_Runner_BaseTestRunner' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/BaseTestRunner.php',
        'PHPUnit_Runner_Exception' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/Exception.php',
        'PHPUnit_Runner_Filter_Factory' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/Filter/Factory.php',
        'PHPUnit_Runner_Filter_GroupFilterIterator' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/Filter/Group.php',
        'PHPUnit_Runner_Filter_Group_Exclude' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/Filter/Group/Exclude.php',
        'PHPUnit_Runner_Filter_Group_Include' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/Filter/Group/Include.php',
        'PHPUnit_Runner_Filter_Test' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/Filter/Test.php',
        'PHPUnit_Runner_StandardTestSuiteLoader' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/StandardTestSuiteLoader.php',
        'PHPUnit_Runner_TestSuiteLoader' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/TestSuiteLoader.php',
        'PHPUnit_Runner_Version' => __DIR__ . '/..' . '/phpunit/phpunit/src/Runner/Version.php',
        'PHPUnit_TextUI_Command' => __DIR__ . '/..' . '/phpunit/phpunit/src/TextUI/Command.php',
        'PHPUnit_TextUI_ResultPrinter' => __DIR__ . '/..' . '/phpunit/phpunit/src/TextUI/ResultPrinter.php',
        'PHPUnit_TextUI_TestRunner' => __DIR__ . '/..' . '/phpunit/phpunit/src/TextUI/TestRunner.php',
        'PHPUnit_Util_Blacklist' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Blacklist.php',
        'PHPUnit_Util_Configuration' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Configuration.php',
        'PHPUnit_Util_ConfigurationGenerator' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/ConfigurationGenerator.php',
        'PHPUnit_Util_ErrorHandler' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/ErrorHandler.php',
        'PHPUnit_Util_Fileloader' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Fileloader.php',
        'PHPUnit_Util_Filesystem' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Filesystem.php',
        'PHPUnit_Util_Filter' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Filter.php',
        'PHPUnit_Util_Getopt' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Getopt.php',
        'PHPUnit_Util_GlobalState' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/GlobalState.php',
        'PHPUnit_Util_InvalidArgumentHelper' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/InvalidArgumentHelper.php',
        'PHPUnit_Util_Log_JSON' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Log/JSON.php',
        'PHPUnit_Util_Log_JUnit' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Log/JUnit.php',
        'PHPUnit_Util_Log_TAP' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Log/TAP.php',
        'PHPUnit_Util_Log_TeamCity' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Log/TeamCity.php',
        'PHPUnit_Util_PHP' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/PHP.php',
        'PHPUnit_Util_PHP_Default' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/PHP/Default.php',
        'PHPUnit_Util_PHP_Windows' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/PHP/Windows.php',
        'PHPUnit_Util_Printer' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Printer.php',
        'PHPUnit_Util_Regex' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Regex.php',
        'PHPUnit_Util_String' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/String.php',
        'PHPUnit_Util_Test' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Test.php',
        'PHPUnit_Util_TestDox_NamePrettifier' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/TestDox/NamePrettifier.php',
        'PHPUnit_Util_TestDox_ResultPrinter' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/TestDox/ResultPrinter.php',
        'PHPUnit_Util_TestDox_ResultPrinter_HTML' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/TestDox/ResultPrinter/HTML.php',
        'PHPUnit_Util_TestDox_ResultPrinter_Text' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/TestDox/ResultPrinter/Text.php',
        'PHPUnit_Util_TestDox_ResultPrinter_XML' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/TestDox/ResultPrinter/XML.php',
        'PHPUnit_Util_TestSuiteIterator' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/TestSuiteIterator.php',
        'PHPUnit_Util_Type' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/Type.php',
        'PHPUnit_Util_XML' => __DIR__ . '/..' . '/phpunit/phpunit/src/Util/XML.php',
        'PHP_Timer' => __DIR__ . '/..' . '/phpunit/php-timer/src/Timer.php',
        'SebastianBergmann\\CodeCoverage\\CoveredCodeNotExecutedException' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Exception/CoveredCodeNotExecutedException.php',
        'SebastianBergmann\\CodeCoverage\\Driver\\Driver' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Driver/Driver.php',
        'SebastianBergmann\\CodeCoverage\\Driver\\HHVM' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Driver/HHVM.php',
        'SebastianBergmann\\CodeCoverage\\Driver\\PHPDBG' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Driver/PHPDBG.php',
        'SebastianBergmann\\CodeCoverage\\Driver\\Xdebug' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Driver/Xdebug.php',
        'SebastianBergmann\\CodeCoverage\\Exception' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Exception/Exception.php',
        'SebastianBergmann\\CodeCoverage\\Filter' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Filter.php',
        'SebastianBergmann\\CodeCoverage\\InvalidArgumentException' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Exception/InvalidArgumentException.php',
        'SebastianBergmann\\CodeCoverage\\MissingCoversAnnotationException' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Exception/MissingCoversAnnotationException.php',
        'SebastianBergmann\\CodeCoverage\\Node\\AbstractNode' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Node/AbstractNode.php',
        'SebastianBergmann\\CodeCoverage\\Node\\Builder' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Node/Builder.php',
        'SebastianBergmann\\CodeCoverage\\Node\\Directory' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Node/Directory.php',
        'SebastianBergmann\\CodeCoverage\\Node\\File' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Node/File.php',
        'SebastianBergmann\\CodeCoverage\\Node\\Iterator' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Node/Iterator.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Clover' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Clover.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Crap4j' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Crap4j.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Html\\Dashboard' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Html/Renderer/Dashboard.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Html\\Directory' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Html/Renderer/Directory.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Html\\Facade' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Html/Facade.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Html\\File' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Html/Renderer/File.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Html\\Renderer' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Html/Renderer.php',
        'SebastianBergmann\\CodeCoverage\\Report\\PHP' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/PHP.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Text' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Text.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Coverage' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Coverage.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Directory' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Directory.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Facade' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Facade.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\File' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/File.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Method' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Method.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Node' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Node.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Project' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Project.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Report' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Report.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Tests' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Tests.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Totals' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Totals.php',
        'SebastianBergmann\\CodeCoverage\\Report\\Xml\\Unit' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Report/Xml/Unit.php',
        'SebastianBergmann\\CodeCoverage\\RuntimeException' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Exception/RuntimeException.php',
        'SebastianBergmann\\CodeCoverage\\UnintentionallyCoveredCodeException' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Exception/UnintentionallyCoveredCodeException.php',
        'SebastianBergmann\\CodeCoverage\\Util' => __DIR__ . '/..' . '/phpunit/php-code-coverage/src/Util.php',
        'SebastianBergmann\\CodeUnitReverseLookup\\Wizard' => __DIR__ . '/..' . '/sebastian/code-unit-reverse-lookup/src/Wizard.php',
        'SebastianBergmann\\Comparator\\ArrayComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/ArrayComparator.php',
        'SebastianBergmann\\Comparator\\Comparator' => __DIR__ . '/..' . '/sebastian/comparator/src/Comparator.php',
        'SebastianBergmann\\Comparator\\ComparisonFailure' => __DIR__ . '/..' . '/sebastian/comparator/src/ComparisonFailure.php',
        'SebastianBergmann\\Comparator\\DOMNodeComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/DOMNodeComparator.php',
        'SebastianBergmann\\Comparator\\DateTimeComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/DateTimeComparator.php',
        'SebastianBergmann\\Comparator\\DoubleComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/DoubleComparator.php',
        'SebastianBergmann\\Comparator\\ExceptionComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/ExceptionComparator.php',
        'SebastianBergmann\\Comparator\\Factory' => __DIR__ . '/..' . '/sebastian/comparator/src/Factory.php',
        'SebastianBergmann\\Comparator\\MockObjectComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/MockObjectComparator.php',
        'SebastianBergmann\\Comparator\\NumericComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/NumericComparator.php',
        'SebastianBergmann\\Comparator\\ObjectComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/ObjectComparator.php',
        'SebastianBergmann\\Comparator\\ResourceComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/ResourceComparator.php',
        'SebastianBergmann\\Comparator\\ScalarComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/ScalarComparator.php',
        'SebastianBergmann\\Comparator\\SplObjectStorageComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/SplObjectStorageComparator.php',
        'SebastianBergmann\\Comparator\\TypeComparator' => __DIR__ . '/..' . '/sebastian/comparator/src/TypeComparator.php',
        'SebastianBergmann\\Diff\\Chunk' => __DIR__ . '/..' . '/sebastian/diff/src/Chunk.php',
        'SebastianBergmann\\Diff\\Diff' => __DIR__ . '/..' . '/sebastian/diff/src/Diff.php',
        'SebastianBergmann\\Diff\\Differ' => __DIR__ . '/..' . '/sebastian/diff/src/Differ.php',
        'SebastianBergmann\\Diff\\LCS\\LongestCommonSubsequence' => __DIR__ . '/..' . '/sebastian/diff/src/LCS/LongestCommonSubsequence.php',
        'SebastianBergmann\\Diff\\LCS\\MemoryEfficientImplementation' => __DIR__ . '/..' . '/sebastian/diff/src/LCS/MemoryEfficientLongestCommonSubsequenceImplementation.php',
        'SebastianBergmann\\Diff\\LCS\\TimeEfficientImplementation' => __DIR__ . '/..' . '/sebastian/diff/src/LCS/TimeEfficientLongestCommonSubsequenceImplementation.php',
        'SebastianBergmann\\Diff\\Line' => __DIR__ . '/..' . '/sebastian/diff/src/Line.php',
        'SebastianBergmann\\Diff\\Parser' => __DIR__ . '/..' . '/sebastian/diff/src/Parser.php',
        'SebastianBergmann\\Environment\\Console' => __DIR__ . '/..' . '/sebastian/environment/src/Console.php',
        'SebastianBergmann\\Environment\\Runtime' => __DIR__ . '/..' . '/sebastian/environment/src/Runtime.php',
        'SebastianBergmann\\Exporter\\Exporter' => __DIR__ . '/..' . '/sebastian/exporter/src/Exporter.php',
        'SebastianBergmann\\GlobalState\\Blacklist' => __DIR__ . '/..' . '/sebastian/global-state/src/Blacklist.php',
        'SebastianBergmann\\GlobalState\\CodeExporter' => __DIR__ . '/..' . '/sebastian/global-state/src/CodeExporter.php',
        'SebastianBergmann\\GlobalState\\Exception' => __DIR__ . '/..' . '/sebastian/global-state/src/Exception.php',
        'SebastianBergmann\\GlobalState\\Restorer' => __DIR__ . '/..' . '/sebastian/global-state/src/Restorer.php',
        'SebastianBergmann\\GlobalState\\RuntimeException' => __DIR__ . '/..' . '/sebastian/global-state/src/RuntimeException.php',
        'SebastianBergmann\\GlobalState\\Snapshot' => __DIR__ . '/..' . '/sebastian/global-state/src/Snapshot.php',
        'SebastianBergmann\\ObjectEnumerator\\Enumerator' => __DIR__ . '/..' . '/sebastian/object-enumerator/src/Enumerator.php',
        'SebastianBergmann\\ObjectEnumerator\\Exception' => __DIR__ . '/..' . '/sebastian/object-enumerator/src/Exception.php',
        'SebastianBergmann\\ObjectEnumerator\\InvalidArgumentException' => __DIR__ . '/..' . '/sebastian/object-enumerator/src/InvalidArgumentException.php',
        'SebastianBergmann\\RecursionContext\\Context' => __DIR__ . '/..' . '/sebastian/recursion-context/src/Context.php',
        'SebastianBergmann\\RecursionContext\\Exception' => __DIR__ . '/..' . '/sebastian/recursion-context/src/Exception.php',
        'SebastianBergmann\\RecursionContext\\InvalidArgumentException' => __DIR__ . '/..' . '/sebastian/recursion-context/src/InvalidArgumentException.php',
        'SebastianBergmann\\ResourceOperations\\ResourceOperations' => __DIR__ . '/..' . '/sebastian/resource-operations/src/ResourceOperations.php',
        'SebastianBergmann\\Version' => __DIR__ . '/..' . '/sebastian/version/src/Version.php',
        'Text_Template' => __DIR__ . '/..' . '/phpunit/php-text-template/src/Template.php',
    );

    public static function getInitializer(ClassLoader $loader)
    {
        return \Closure::bind(function () use ($loader) {
            $loader->prefixLengthsPsr4 = ComposerStaticInitd67eef4e653bb27f765e27b84e0b5425::$prefixLengthsPsr4;
            $loader->prefixDirsPsr4 = ComposerStaticInitd67eef4e653bb27f765e27b84e0b5425::$prefixDirsPsr4;
            $loader->prefixesPsr0 = ComposerStaticInitd67eef4e653bb27f765e27b84e0b5425::$prefixesPsr0;
            $loader->classMap = ComposerStaticInitd67eef4e653bb27f765e27b84e0b5425::$classMap;

        }, null, ClassLoader::class);
    }
}
