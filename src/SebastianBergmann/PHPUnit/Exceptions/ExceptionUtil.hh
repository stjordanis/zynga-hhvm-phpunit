<?hh // strict

namespace SebastianBergmann\PHPUnit\Exceptions;

use SebastianBergmann\Comparator\ComparisonFailure;
use SebastianBergmann\PHPUnit\Exceptions\ErrorException;
use SebastianBergmann\PHPUnit\Exceptions\ExceptionWrapper;
use SebastianBergmann\PHPUnit\Exceptions\ExpectationFailedException;
use SebastianBergmann\PHPUnit\Interfaces\SelfDescribingInterface;
use \Exception;

class ExceptionUtil {
  /**
   * Returns a description for an exception.
   *
   * @param Exception $e
   *
   * @return string
   *
   * @since Method available since Release 3.2.0
   */
  public static function exceptionToString(Exception $e): string {

    $buffer = '';

    if ($e instanceof SelfDescribingInterface) {

      $buffer = $e->toString();

      if ($e instanceof ExpectationFailedException) {
        $comparisonFailure = $e->getComparisonFailure();

        if ($comparisonFailure instanceof ComparisonFailure) {
          $buffer = $buffer.$comparisonFailure->getDiff();
        }
      }

      if ($buffer != '') {
        $buffer = trim($buffer)."\n";
      }

    } else if ($e instanceof ErrorException) {
      $buffer = $e->getMessage()."\n";
    } else if ($e instanceof ExceptionWrapper) {
      $buffer = $e->getClassname().': '.$e->getMessage()."\n";
    } else {
      $buffer = get_class($e).': '.$e->getMessage()."\n";
    }

    return $buffer;
  }

}
