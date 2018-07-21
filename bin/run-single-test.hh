#!/usr/local/bin/hhvm
<?hh

function usage($message) {
  echo "Error: $message\n";
  echo "\n";
  echo "run-single-test.hh <path>Test.hh [testFunction]\n";
  echo "\n";
  echo " Runs a single test suite handles the phpunit arguments\n";
  echo "\n";
  exit(255);
}

$projectRoot = dirname(dirname(dirname(dirname(dirname(__FILE__)))));

$filePath = null;

if ( isset($argv[1]) ) {
  $filePath = $argv[1];
} else {
  usage('No test file path provided');
}

$testFunction = '';

if ( isset($argv[2]) ) {
  $testFunction = $argv[2];
}

$testName = '';

if ( preg_match( '/\/([a-zA-Z0-9]*Test)\.hh$/', $filePath, $pregs) ) {
  $testName = $pregs[1];
} else {
  usage("Expecting a path ending in <TestName>Test.hh");
}

$command = array();
$command[] = $projectRoot . '/vendor/bin/phpunit';
$command[] = '--debug';

// add filter to get only a single function
if ( $testFunction != '' ) {
  $command[] = '--filter \'/::' . $testFunction . '$/\'';
}

$command[] = $testName;
$command[] = $filePath;

$cmd = join(' ', $command);

echo "command: $cmd\n";
passthru($cmd);
