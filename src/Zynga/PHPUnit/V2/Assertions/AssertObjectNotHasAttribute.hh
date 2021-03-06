<?hh // strict

namespace Zynga\PHPUnit\V2\Assertions;

use Zynga\PHPUnit\V2\Assertions;
use Zynga\PHPUnit\V2\Constraints\NotConstraint;
use Zynga\PHPUnit\V2\Constraints\ObjectHasAttributeConstraint;
use Zynga\PHPUnit\V2\Exceptions\InvalidArgumentExceptionFactory;

class AssertObjectNotHasAttribute {

  /**
   * Asserts that an object does not have a specified attribute.
   *
   * @param string $attributeName
   * @param object $object
   * @param string $message
   *
   * @since Method available since Release 3.0.0
   */
  public static function evaluate(
    Assertions $assertions,
    string $attributeName,
    mixed $object,
    string $message = '',
  ): bool {

    if (!is_string($attributeName)) {
      throw InvalidArgumentExceptionFactory::factory(1, 'string');
    }

    if (!preg_match(
          '/[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*/',
          $attributeName,
        )) {
      throw InvalidArgumentExceptionFactory::factory(
        1,
        'valid attribute name',
      );
    }

    if (!is_object($object)) {
      throw InvalidArgumentExceptionFactory::factory(2, 'object');
    }

    $attrConstraint = new ObjectHasAttributeConstraint();
    $attrConstraint->setExpected($attributeName);

    $notConstraint = new NotConstraint();
    $notConstraint->setExpected($attrConstraint);

    return $assertions->assertThat($object, $notConstraint, $message);

  }

}
