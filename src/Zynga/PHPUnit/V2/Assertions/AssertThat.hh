<?hh // strict

namespace Zynga\PHPUnit\V2\Assertions;

use Zynga\PHPUnit\V2\Assertions;
use Zynga\PHPUnit\V2\Exceptions\AssertionFailedException;
use Zynga\PHPUnit\V2\Exceptions\ExpectationFailedException;
use Zynga\PHPUnit\V2\Interfaces\ConstraintInterface;

class AssertThat {

  /**
   * Evaluates a PHPUnit_Framework_Constraint matcher object.
   *
   * @param mixed                        $value
   * @param PHPUnit_Framework_Constraint $constraint
   * @param string                       $message
   *
   * @since Method available since Release 3.0.0
   */
  final static public function evaluate(
    Assertions $assertions,
    mixed $value,
    ConstraintInterface $constraint,
    string $message = '',
  ): bool {

    $assertions->counter()->increment($constraint->count());

    try {
      return $constraint->evaluate($value, $message, true);
    } catch (ExpectationFailedException $e) {
      throw new AssertionFailedException($e->getMessage(), $e->getCode(), $e);
    }

  }

}
