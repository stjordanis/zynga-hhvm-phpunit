<?hh // strict

namespace SebastianBergmann\PHPUnit\Assertions;

use SebastianBergmann\PHPUnit\Assertions;
use SebastianBergmann\PHPUnit\Constraints\GreaterThanConstraint;

class AssertGreaterThan {

  /**
   * Asserts that a value is greater than another value.
   *
   * @param mixed  $expected
   * @param mixed  $actual
   * @param string $message
   *
   * @since Method available since Release 3.1.0
   */
  public static function evaluate(
    Assertions $assertions,
    mixed $expected,
    mixed $actual,
    string $message = '',
  ): bool {

    $constraint = new GreaterThanConstraint();
    $constraint->setExpected($expected);

    return $assertions->assertThat($actual, $constraint, $message);

  }

}
