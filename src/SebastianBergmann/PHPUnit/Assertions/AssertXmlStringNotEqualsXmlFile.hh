<?hh // strict

namespace SebastianBergmann\PHPUnit\Assertions;

use SebastianBergmann\PHPUnit\Assertions;
use SebastianBergmann\PHPUnit\Environment\XML;

class AssertXmlStringNotEqualsXmlFile {

  /**
   * Asserts that two XML documents are not equal.
   *
   * @param string $expectedFile
   * @param string $actualXml
   * @param string $message
   *
   * @since Method available since Release 3.3.0
   */
  public static function evaluate(
    Assertions $assertions,
    string $expectedFile,
    string $actualXml,
    string $message = '',
  ): bool {

    $expected = XML::loadFile($expectedFile);
    $actual = XML::load($actualXml);

    return $assertions->assertNotEquals($expected, $actual, $message);

  }

}
