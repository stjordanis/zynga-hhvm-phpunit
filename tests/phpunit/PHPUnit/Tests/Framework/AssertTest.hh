<?hh

namespace PHPUnit\Tests\Framework;

/*
 * This file is part of PHPUnit.
 *
 * (c) Sebastian Bergmann <sebastian@phpunit.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

// Classes associated to phpunit or other frameworks;
use PHPUnit\Tests\Mock\Author;
use PHPUnit\Tests\Mock\Book;
use PHPUnit\Tests\Mock\ClassWithNonPublicAttributes;
use PHPUnit\Tests\Mock\ClassWithToString;
use PHPUnit\Tests\Mock\SampleClass;
use PHPUnit\Tests\Mock\Struct;
use PHPUnit\Tests\Mock\TestIterator;
use Zynga\Framework\Environment\CodePath\V1\CodePath;

use SebastianBergmann\PHPUnit\Exceptions\AssertionFailedException;
use SebastianBergmann\PHPUnit\Exceptions\InvalidArgumentException;

use \PHPUnit_Framework_AssertionFailedError;
use \PHPUnit_Framework_ExpectationFailedException;
use \PHPUnit_Framework_Exception;
use \PHPUnit_Framework_IncompleteTestError;
use \PHPUnit_Framework_SkippedTestError;
use \PHPUnit_Framework_TestCase;
use \PHPUnit_Util_XML;

// External classes:
use \ArrayIterator;
use \DateTime;
use \DateTimeZone;
use \DOMDocument;
use \Exception;
use \SplObjectStorage;
use \stdClass;

/**
 * @since      Class available since Release 2.0.0
 */
class AssertTest extends PHPUnit_Framework_TestCase {

  public function doSetUpBeforeClass(): void {
    // noop
  }

  public function doTearDownAfterClass(): void {
    // noop
  }

  protected function getFilesDirectory(): string {
    return
      CodePath::getRoot().
      DIRECTORY_SEPARATOR.
      'vendor'.
      DIRECTORY_SEPARATOR.
      'zynga'.
      DIRECTORY_SEPARATOR.
      'phpunit'.
      DIRECTORY_SEPARATOR.
      'tests'.
      DIRECTORY_SEPARATOR.
      'phpunit'.
      DIRECTORY_SEPARATOR.
      '_files'.
      DIRECTORY_SEPARATOR;
  }

  public function testFail(): void {
    try {
      $this->fail();
    } catch (AssertionFailedException $e) {
      $this->assertTrue(true);
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      $this->assertTrue(true);
      return;
    }
    throw new AssertionFailedException('Fail did not throw fail exception');
  }

  public function testAssertSplObjectStorageContainsObject() {
    $a = new stdClass();
    $b = new stdClass();
    $c = new SplObjectStorage();
    $c->attach($a);

    $this->assertContains($a, $c);

    try {
      $this->assertContains($b, $c);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArrayContainsObject(): void {
    $a = new stdClass();
    $b = new stdClass();

    $this->assertContains($a, [$a]);

    try {
      $this->assertContains($a, [$b]);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail('Expected exceptions for test were not thrown');
  }

  public function testAssertArrayContainsString(): void {
    $this->assertContains('foo', ['foo']);

    try {
      $this->assertContains('foo', ['bar']);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArrayContainsNonObject(): void {
    $this->assertContains('foo', [true]);

    try {
      $this->assertContains('foo', [true], '', false, true, true);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertContainsOnlyInstancesOf(): void {
    $test = [new Book(), new Book()];
    $this->assertContainsOnlyInstancesOf(Book::class, $test);
    $this->assertContainsOnlyInstancesOf(stdClass::class, [new stdClass()]);

    $test2 = [new Author('Test')];
    try {
      $this->assertContainsOnlyInstancesOf(Book::class, $test2);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }
    $this->fail();
  }

  public function testAssertArrayHasIntegerKey() {
    $this->assertArrayHasKey(0, ['foo']);

    try {
      $this->assertArrayHasKey(1, ['foo']);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArraySubset() {
    $array = [
      'a' => 'item a',
      'b' => 'item b',
      'c' => ['a2' => 'item a2', 'b2' => 'item b2'],
      'd' => ['a2' => ['a3' => 'item a3', 'b3' => 'item b3']],
    ];

    $this->assertArraySubset(
      ['a' => 'item a', 'c' => ['a2' => 'item a2']],
      $array,
    );
    $this->assertArraySubset(
      ['a' => 'item a', 'd' => ['a2' => ['b3' => 'item b3']]],
      $array,
    );

    try {
      $this->assertArraySubset(['a' => 'bad value'], $array);
    } catch (AssertionFailedException $e) {
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
    }

    try {
      $this->assertArraySubset(
        ['d' => ['a2' => ['bad index' => 'item b3']]],
        $array,
      );
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArraySubsetWithDeepNestedArrays() {
    $array = ['path' => ['to' => ['the' => ['cake' => 'is a lie']]]];

    $this->assertArraySubset(['path' => []], $array);
    $this->assertArraySubset(['path' => ['to' => []]], $array);
    $this->assertArraySubset(['path' => ['to' => ['the' => []]]], $array);
    $this->assertArraySubset(
      ['path' => ['to' => ['the' => ['cake' => 'is a lie']]]],
      $array,
    );

    try {
      $this->assertArraySubset(
        ['path' => ['to' => ['the' => ['cake' => 'is not a lie']]]],
        $array,
      );
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArraySubsetWithNoStrictCheckAndObjects() {
    $obj = new stdClass();
    $reference = &$obj;
    $array = ['a' => $obj];

    $this->assertArraySubset(['a' => $reference], $array);
    $this->assertArraySubset(['a' => new stdClass()], $array);
  }

  public function testAssertArraySubsetWithStrictCheckAndObjects() {
    $obj = new stdClass();
    $reference = &$obj;
    $array = ['a' => $obj];

    $this->assertArraySubset(['a' => $reference], $array, true);

    try {
      $this->assertArraySubset(['a' => new stdClass()], $array, true);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail('Strict recursive array check fail.');
  }

  /**
   * @dataProvider assertArraySubsetInvalidArgumentProvider
   */
  public function testAssertArraySubsetRaisesExceptionForInvalidArguments(
    $partial,
    $subject,
  ) {
    $message = 'array or ArrayAccess';
    try {
      $this->assertArraySubset($partial, $subject);
    } catch (PHPUnit_framework_Exception $e) {
      $this->assertStringEndsWith($message, $e->getMessage());
      return;
    } catch (InvalidArgumentException $e) {
      $this->assertStringEndsWith($message, $e->getMessage());
      return;
    }
    $this->fail();
  }

  /**
   * @return array
   */
  public function assertArraySubsetInvalidArgumentProvider() {
    return [[false, []], [[], false]];
  }

  // JEO: invalid test function sig arraykey, array, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertArrayNotHasKey
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertArrayNotHasKeyThrowsExceptionForInvalidFirstArgument()
  // {
  //     $this->assertArrayNotHasKey(null, []);
  // }

  // JEO: invalid test function sig arraykey, array, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertArrayNotHasKey
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertArrayNotHasKeyThrowsExceptionForInvalidSecondArgument()
  // {
  //     $this->assertArrayNotHasKey(0, null);
  // }

  public function testAssertArrayNotHasIntegerKey() {
    $this->assertArrayNotHasKey(1, ['foo']);

    try {
      $this->assertArrayNotHasKey(0, ['foo']);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArrayHasStringKey() {
    $this->assertArrayHasKey('foo', ['foo' => 'bar']);

    try {
      $this->assertArrayHasKey('bar', ['foo' => 'bar']);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArrayNotHasStringKey() {

    $this->assertArrayNotHasKey('bar', ['foo' => 'bar']);

    try {
      $this->assertArrayNotHasKey('foo', ['foo' => 'bar']);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig arraykey, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertArrayHasKey
  //  */
  // public function testAssertArrayHasKeyAcceptsArrayObjectValue()
  // {
  //     $array        = new ArrayObject();
  //     $array['foo'] = 'bar';
  //     $this->assertArrayHasKey('foo', $array);
  // }

  // JEO: invalid test function sig arraykey, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertArrayHasKey
  //  * @expectedException PHPUnit_Framework_AssertionFailedError
  //  */
  // public function testAssertArrayHasKeyProperlyFailsWithArrayObjectValue()
  // {
  //     $array        = new ArrayObject();
  //     $array['bar'] = 'bar';
  //     $this->assertArrayHasKey('foo', $array);
  // }

  // JEO: invalid test function sig arraykey, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertArrayHasKey
  //  */
  // public function testAssertArrayHasKeyAcceptsArrayAccessValue()
  // {
  //     $array        = new SampleArrayAccess();
  //     $array['foo'] = 'bar';
  //     $this->assertArrayHasKey('foo', $array);
  // }

  // JEO: invalid test function sig arraykey, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertArrayHasKey
  //  * @expectedException PHPUnit_Framework_AssertionFailedError
  //  */
  // public function testAssertArrayHasKeyProperlyFailsWithArrayAccessValue()
  // {
  //     $array        = new SampleArrayAccess();
  //     $array['bar'] = 'bar';
  //     $this->assertArrayHasKey('foo', $array);
  // }

  // JEO: invalid test function sig arraykey, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertArrayNotHasKey
  //  */
  // public function testAssertArrayNotHasKeyAcceptsArrayAccessValue()
  // {
  //     $array        = new ArrayObject();
  //     $array = array();
  //     $array['foo'] = 'bar';
  //     $this->assertArrayNotHasKey('bar', $array);
  // }

  // JEO: invalid test function sig arraykey, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertArrayNotHasKey
  //  * @expectedException PHPUnit_Framework_AssertionFailedError
  //  */
  // public function testAssertArrayNotHasKeyPropertlyFailsWithArrayAccessValue()
  // {
  //     $array        = new ArrayObject();
  //     $array = array();
  //     $array['bar'] = 'bar';
  //     $this->assertArrayNotHasKey('bar', $array);
  // }

  public function testAssertContainsThrowsException() {
    try {
      $this->assertContains(null, null);
    } catch (InvalidArgumentException $e) {
      $this->assertTrue(true);
      return;
    } catch (PHPUnit_Framework_Exception $e) {
      $this->assertTrue(true);
      return;
    }
    $this->fail();
  }

  public function testAssertIteratorContainsObject() {
    $foo = new stdClass();

    $this->assertContains($foo, new TestIterator([$foo]));

    try {
      $this->assertContains($foo, new TestIterator([new stdClass()]));
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertIteratorContainsString() {

    $this->assertContains('foo', new TestIterator(['foo']));

    try {
      $this->assertContains('foo', new TestIterator(['bar']));
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertStringContainsString() {
    $this->assertContains('foo', 'foobar');

    try {
      $this->assertContains('foo', 'bar');
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertNotContainsThrowsException() {
    try {
      $this->assertNotContains(null, null);
    } catch (InvalidArgumentException $e) {
      $this->assertTrue(true);
      return;
    } catch (PHPUnit_Framework_Exception $e) {
      $this->assertTrue(true);
      return;
    }
    $this->fail('Failed to find an acceptable exception.');
  }

  public function testAssertSplObjectStorageNotContainsObject() {
    $a = new stdClass();
    $b = new stdClass();
    $c = new SplObjectStorage();
    $c->attach($a);

    $this->assertNotContains($b, $c);

    try {
      $this->assertNotContains($a, $c);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArrayNotContainsObject() {
    $a = new stdClass();
    $b = new stdClass();

    $this->assertNotContains($a, [$b]);

    try {
      $this->assertNotContains($a, [$a]);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArrayNotContainsString() {
    $this->assertNotContains('foo', ['bar']);

    try {
      $this->assertNotContains('foo', ['foo']);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArrayNotContainsNonObject() {
    $this->assertNotContains('foo', [true], '', false, true, true);

    try {
      $this->assertNotContains('foo', [true]);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertStringNotContainsString() {
    $this->assertNotContains('foo', 'bar');

    try {
      $this->assertNotContains('foo', 'foo');
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig string, mixed, bool, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertContainsOnly
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertContainsOnlyThrowsException()
  // {
  //     $this->assertContainsOnly(null, null);
  // }

  // JEO: invalid test function sig string, mixed, bool, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertNotContainsOnly
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertNotContainsOnlyThrowsException()
  // {
  //     $this->assertNotContainsOnly(null, null);
  // }

  // JEO: invalid test function sig classname<T>, mixed, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertContainsOnlyInstancesOf
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertContainsOnlyInstancesOfThrowsException()
  // {
  //     $this->assertContainsOnlyInstancesOf(null, null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertContainsOnly
   */
  public function testAssertArrayContainsOnlyIntegers() {
    $this->assertContainsOnly('integer', [1, 2, 3]);

    try {
      $this->assertContainsOnly('integer', ['1', 2, 3]);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail('Failed to catch appropriate exceptions.');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotContainsOnly
   */
  public function testAssertArrayNotContainsOnlyIntegers() {
    $this->assertNotContainsOnly('integer', ['1', 2, 3]);

    try {
      $this->assertNotContainsOnly('integer', [1, 2, 3]);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertArrayContainsOnlyStdClass() {
    $this->assertContainsOnly('StdClass', [new stdClass()]);

    try {
      $this->assertContainsOnly('StdClass', ['StdClass']);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    } catch (AssertionFailedException $e) {
      return;
    }

    $this->fail('Failed to catch appropriate exceptions.');

  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotContainsOnly
   */
  public function testAssertArrayNotContainsOnlyStdClass() {
    $this->assertNotContainsOnly('StdClass', ['StdClass']);

    try {
      $this->assertNotContainsOnly('StdClass', [new stdClass()]);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  protected function sameValues() {
    $object = new SampleClass(4, 8, 15);
    // cannot use $filesDirectory, because neither setUp() nor
    // setUpBeforeClass() are executed before the data providers
    $file = $this->getFilesDirectory().DIRECTORY_SEPARATOR.'foo.xml';
    $resource = fopen($file, 'r');

    return [
      // null
      [null, null],
      // strings
      ['a', 'a'],
      // integers
      [0, 0],
      // floats
      [2.3, 2.3],
      [1 / 3, 1 - 2 / 3],
      [log(0), log(0)],
      // arrays
      [[], []],
      [[0 => 1], [0 => 1]],
      [[0 => null], [0 => null]],
      // JEO: type checker does not let you make an array that mixes use
      // for array contexts
      // [['a', 'b' => [1, 2]], ['a', 'b' => [1, 2]]],
      // objects
      [$object, $object],
      // resources
      [$resource, $resource],
    ];
  }

  protected function notEqualValues() {
    // cyclic dependencies
    $book1 = new Book();
    $book1->author = new Author('Terry Pratchett');
    $book1->author->books[] = $book1;
    $book2 = new Book();
    $book2->author = new Author('Terry Pratch');
    $book2->author->books[] = $book2;

    $book3 = new Book();
    $book3->author = new Author('Terry Pratchett');
    $book4 = new stdClass();
    $book4->author = new Author('Terry Pratchett');

    $object1 = new SampleClass(4, 8, 15);
    $object2 = new SampleClass(16, 23, 42);
    $object3 = new SampleClass(4, 8, 15);
    $storage1 = new SplObjectStorage();
    $storage1->attach($object1);
    $storage2 = new SplObjectStorage();
    $storage2->attach($object3); // same content, different object

    // cannot use $filesDirectory, because neither setUp() nor
    // setUpBeforeClass() are executed before the data providers
    $file = $this->getFilesDirectory().'foo.xml';

    return [
      // strings
      ['a', 'b'],
      ['a', 'A'],
      // https://github.com/sebastianbergmann/phpunit/issues/1023
      ['9E6666666', '9E7777777'],
      // integers
      [1, 2],
      [2, 1],
      // floats
      [2.3, 4.2],
      [2.3, 4.2, 0.5],
      [[2.3], [4.2], 0.5],
      [[[2.3]], [[4.2]], 0.5],
      [new Struct(2.3), new Struct(4.2), 0.5],
      [[new Struct(2.3)], [new Struct(4.2)], 0.5],
      // NAN
      [NAN, NAN],
      // arrays
      [[], [0 => 1]],
      [[0 => 1], []],
      [[0 => null], []],
      [[0 => 1, 1 => 2], [0 => 1, 1 => 3]],
      // Type checker does not let you make array that mixes context.
      // [['a', 'b' => [1, 2]], ['a', 'b' => [2, 1]]],
      // objects
      [new SampleClass(4, 8, 15), new SampleClass(16, 23, 42)],
      [$object1, $object2],
      [$book1, $book2],
      [$book3, $book4], // same content, different class
      // resources
      [fopen($file, 'r'), fopen($file, 'r')],
      // SplObjectStorage
      [$storage1, $storage2],
      // DOMDocument
      [
        PHPUnit_Util_XML::load('<root></root>'),
        PHPUnit_Util_XML::load('<bar/>'),
      ],
      [
        PHPUnit_Util_XML::load('<foo attr1="bar"/>'),
        PHPUnit_Util_XML::load('<foo attr1="foobar"/>'),
      ],
      [
        PHPUnit_Util_XML::load('<foo> bar </foo>'),
        PHPUnit_Util_XML::load('<foo />'),
      ],
      [
        PHPUnit_Util_XML::load('<foo xmlns="urn:myns:bar"/>'),
        PHPUnit_Util_XML::load('<foo xmlns="urn:notmyns:bar"/>'),
      ],
      [
        PHPUnit_Util_XML::load('<foo> bar </foo>'),
        PHPUnit_Util_XML::load('<foo> bir </foo>'),
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 03:13:35',
          new DateTimeZone('America/New_York'),
        ),
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 03:13:35',
          new DateTimeZone('America/New_York'),
        ),
        3500,
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 05:13:35',
          new DateTimeZone('America/New_York'),
        ),
        3500,
      ],
      [
        new DateTime('2013-03-29', new DateTimeZone('America/New_York')),
        new DateTime('2013-03-30', new DateTimeZone('America/New_York')),
      ],
      [
        new DateTime('2013-03-29', new DateTimeZone('America/New_York')),
        new DateTime('2013-03-30', new DateTimeZone('America/New_York')),
        43200,
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/Chicago'),
        ),
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/Chicago'),
        ),
        3500,
      ],
      [
        new DateTime('2013-03-30', new DateTimeZone('America/New_York')),
        new DateTime('2013-03-30', new DateTimeZone('America/Chicago')),
      ],
      [
        new DateTime('2013-03-29T05:13:35-0600'),
        new DateTime('2013-03-29T04:13:35-0600'),
      ],
      [
        new DateTime('2013-03-29T05:13:35-0600'),
        new DateTime('2013-03-29T05:13:35-0500'),
      ],
      // Exception
      //array(new Exception('Exception 1'), new Exception('Exception 2')),
      // different types
      [new SampleClass(4, 8, 15), false],
      [false, new SampleClass(4, 8, 15)],
      [[0 => 1, 1 => 2], false],
      [false, [0 => 1, 1 => 2]],
      [[], new stdClass()],
      [new stdClass(), []],
      // PHP: 0 == 'Foobar' => true!
      // We want these values to differ
      [0, 'Foobar'],
      ['Foobar', 0],
      [3, acos(8)],
      [acos(8), 3],
    ];
  }

  protected function equalValues() {
    // cyclic dependencies
    $book1 = new Book();
    $book1->author = new Author('Terry Pratchett');
    $book1->author->books[] = $book1;
    $book2 = new Book();
    $book2->author = new Author('Terry Pratchett');
    $book2->author->books[] = $book2;

    $object1 = new SampleClass(4, 8, 15);
    $object2 = new SampleClass(4, 8, 15);
    $storage1 = new SplObjectStorage();
    $storage1->attach($object1);
    $storage2 = new SplObjectStorage();
    $storage2->attach($object1);

    return [
      // strings
      ['a', 'A', 0, false, true], // ignore case
      // arrays
      [['a' => 1, 'b' => 2], ['b' => 2, 'a' => 1]],
      [[1], ['1']],
      [[3, 2, 1], [2, 3, 1], 0, true], // canonicalized comparison
      // floats
      [2.3, 2.5, 0.5],
      [[2.3], [2.5], 0.5],
      [[[2.3]], [[2.5]], 0.5],
      [new Struct(2.3), new Struct(2.5), 0.5],
      [[new Struct(2.3)], [new Struct(2.5)], 0.5],
      // numeric with delta
      [1, 2, 1],
      // objects
      [$object1, $object2],
      [$book1, $book2],
      // SplObjectStorage
      [$storage1, $storage2],
      // DOMDocument
      [
        PHPUnit_Util_XML::load('<root></root>'),
        PHPUnit_Util_XML::load('<root/>'),
      ],
      [
        PHPUnit_Util_XML::load('<root attr="bar"></root>'),
        PHPUnit_Util_XML::load('<root attr="bar"/>'),
      ],
      [
        PHPUnit_Util_XML::load('<root><foo attr="bar"></foo></root>'),
        PHPUnit_Util_XML::load('<root><foo attr="bar"/></root>'),
      ],
      [
        PHPUnit_Util_XML::load("<root>\n  <child/>\n</root>"),
        PHPUnit_Util_XML::load('<root><child/></root>'),
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 04:13:25',
          new DateTimeZone('America/New_York'),
        ),
        10,
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 04:14:40',
          new DateTimeZone('America/New_York'),
        ),
        65,
      ],
      [
        new DateTime('2013-03-29', new DateTimeZone('America/New_York')),
        new DateTime('2013-03-29', new DateTimeZone('America/New_York')),
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 03:13:35',
          new DateTimeZone('America/Chicago'),
        ),
      ],
      [
        new DateTime(
          '2013-03-29 04:13:35',
          new DateTimeZone('America/New_York'),
        ),
        new DateTime(
          '2013-03-29 03:13:49',
          new DateTimeZone('America/Chicago'),
        ),
        15,
      ],
      [
        new DateTime('2013-03-30', new DateTimeZone('America/New_York')),
        new DateTime(
          '2013-03-29 23:00:00',
          new DateTimeZone('America/Chicago'),
        ),
      ],
      [
        new DateTime('2013-03-30', new DateTimeZone('America/New_York')),
        new DateTime(
          '2013-03-29 23:01:30',
          new DateTimeZone('America/Chicago'),
        ),
        100,
      ],
      [
        new DateTime('@1364616000'),
        new DateTime(
          '2013-03-29 23:00:00',
          new DateTimeZone('America/Chicago'),
        ),
      ],
      [
        new DateTime('2013-03-29T05:13:35-0500'),
        new DateTime('2013-03-29T04:13:35-0600'),
      ],
      // Exception
      //array(new Exception('Exception 1'), new Exception('Exception 1')),
      // mixed types
      [0, '0'],
      ['0', 0],
      [2.3, '2.3'],
      ['2.3', 2.3],
      [(string) (1 / 3), 1 - 2 / 3],
      [1 / 3, (string) (1 - 2 / 3)],
      ['string representation', new ClassWithToString()],
      [new ClassWithToString(), 'string representation'],
    ];
  }

  public function equalProvider() {
    // same |= equal
    return array_merge($this->equalValues(), $this->sameValues());
  }

  public function notEqualProvider() {
    return $this->notEqualValues();
  }

  public function sameProvider() {
    return $this->sameValues();
  }

  public function notSameProvider() {
    // not equal |= not same
    // equal, ¬same |= not same
    return array_merge($this->notEqualValues(), $this->equalValues());
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertEquals
   * @dataProvider equalProvider
   */
  public function testAssertEqualsSucceeds(
    $a,
    $b,
    $delta = 0.0,
    $canonicalize = false,
    $ignoreCase = false,
  ) {
    $this->assertEquals($a, $b, '', $delta, 10, $canonicalize, $ignoreCase);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertEquals
   * @dataProvider notEqualProvider
   */
  public function testAssertEqualsFails(
    $a,
    $b,
    $delta = 0.0,
    $canonicalize = false,
    $ignoreCase = false,
  ) {
    try {
      $this->assertEquals($a, $b, '', $delta, 10, $canonicalize, $ignoreCase);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotEquals
   * @dataProvider notEqualProvider
   */
  public function testAssertNotEqualsSucceeds(
    $a,
    $b,
    $delta = 0.0,
    $canonicalize = false,
    $ignoreCase = false,
  ) {
    $this->assertNotEquals(
      $a,
      $b,
      '',
      $delta,
      10,
      $canonicalize,
      $ignoreCase,
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotEquals
   * @dataProvider equalProvider
   */
  public function testAssertNotEqualsFails(
    $a,
    $b,
    $delta = 0.0,
    $canonicalize = false,
    $ignoreCase = false,
  ) {
    try {
      $this->assertNotEquals(
        $a,
        $b,
        '',
        $delta,
        10,
        $canonicalize,
        $ignoreCase,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertSame
   * @dataProvider sameProvider
   */
  public function testAssertSameSucceeds($a, $b) {
    $this->assertSame($a, $b);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertSame
   * @dataProvider notSameProvider
   */
  public function testAssertSameFails($a, $b) {
    try {
      $this->assertSame($a, $b);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotSame
   * @dataProvider notSameProvider
   */
  public function testAssertNotSameSucceeds($a, $b) {
    $this->assertNotSame($a, $b);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotSame
   * @dataProvider sameProvider
   */
  public function testAssertNotSameFails($a, $b) {
    try {
      $this->assertNotSame($a, $b);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertXmlFileEqualsXmlFile
   */
  public function testAssertXmlFileEqualsXmlFile() {
    $this->assertXmlFileEqualsXmlFile(
      $this->getFilesDirectory().'foo.xml',
      $this->getFilesDirectory().'foo.xml',
    );

    try {
      $this->assertXmlFileEqualsXmlFile(
        $this->getFilesDirectory().'foo.xml',
        $this->getFilesDirectory().'bar.xml',
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertXmlFileNotEqualsXmlFile
   */
  public function testAssertXmlFileNotEqualsXmlFile() {
    $this->assertXmlFileNotEqualsXmlFile(
      $this->getFilesDirectory().'foo.xml',
      $this->getFilesDirectory().'bar.xml',
    );

    try {
      $this->assertXmlFileNotEqualsXmlFile(
        $this->getFilesDirectory().'foo.xml',
        $this->getFilesDirectory().'foo.xml',
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertXmlStringEqualsXmlFile
   */
  public function testAssertXmlStringEqualsXmlFile() {
    $this->assertXmlStringEqualsXmlFile(
      $this->getFilesDirectory().'foo.xml',
      file_get_contents($this->getFilesDirectory().'foo.xml'),
    );

    try {
      $this->assertXmlStringEqualsXmlFile(
        $this->getFilesDirectory().'foo.xml',
        file_get_contents($this->getFilesDirectory().'bar.xml'),
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertXmlStringNotEqualsXmlFile
   */
  public function testXmlStringNotEqualsXmlFile() {
    $this->assertXmlStringNotEqualsXmlFile(
      $this->getFilesDirectory().'foo.xml',
      file_get_contents($this->getFilesDirectory().'bar.xml'),
    );

    try {
      $this->assertXmlStringNotEqualsXmlFile(
        $this->getFilesDirectory().'foo.xml',
        file_get_contents($this->getFilesDirectory().'foo.xml'),
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertXmlStringEqualsXmlString
   */
  public function testAssertXmlStringEqualsXmlString() {
    $this->assertXmlStringEqualsXmlString('<root/>', '<root/>');

    try {
      $this->assertXmlStringEqualsXmlString('<foo/>', '<bar/>');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @expectedException PHPUnit_Framework_Exception
   * @covers            PHPUnit_Framework_Assert::assertXmlStringEqualsXmlString
   * @ticket            1860
   */
  public function testAssertXmlStringEqualsXmlString2() {
    $this->assertXmlStringEqualsXmlString('<a></b>', '<c></d>');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertXmlStringEqualsXmlString
   * @ticket 1860
   */
  public function testAssertXmlStringEqualsXmlString3() {
    $expected = <<<XML
<?xml version="1.0"?>
<root>
    <node />
</root>
XML;

    $actual = <<<XML
<?xml version="1.0"?>
<root>
<node />
</root>
XML;

    $this->assertXmlStringEqualsXmlString($expected, $actual);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertXmlStringNotEqualsXmlString
   */
  public function testAssertXmlStringNotEqualsXmlString() {
    $this->assertXmlStringNotEqualsXmlString('<foo/>', '<bar/>');

    try {
      $this->assertXmlStringNotEqualsXmlString('<root/>', '<root/>');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertEqualXMLStructure
   */
  public function testXMLStructureIsSame() {
    $expected = new DOMDocument();
    $expected->load($this->getFilesDirectory().'structureExpected.xml');

    $actual = new DOMDocument();
    $actual->load($this->getFilesDirectory().'structureExpected.xml');

    $this->assertEqualXMLStructure(
      $expected->firstChild,
      $actual->firstChild,
      true,
    );
  }

  /**
   * @covers            PHPUnit_Framework_Assert::assertEqualXMLStructure
   * @expectedException PHPUnit_Framework_ExpectationFailedException
   */
  public function testXMLStructureWrongNumberOfAttributes() {
    $expected = new DOMDocument();
    $expected->load($this->getFilesDirectory().'structureExpected.xml');

    $actual = new DOMDocument();
    $actual->load(
      $this->getFilesDirectory().'structureWrongNumberOfAttributes.xml',
    );

    $this->assertEqualXMLStructure(
      $expected->firstChild,
      $actual->firstChild,
      true,
    );
  }

  /**
   * @covers            PHPUnit_Framework_Assert::assertEqualXMLStructure
   * @expectedException PHPUnit_Framework_ExpectationFailedException
   */
  public function testXMLStructureWrongNumberOfNodes() {
    $expected = new DOMDocument();
    $expected->load($this->getFilesDirectory().'structureExpected.xml');

    $actual = new DOMDocument();
    $actual->load(
      $this->getFilesDirectory().'structureWrongNumberOfNodes.xml',
    );

    $this->assertEqualXMLStructure(
      $expected->firstChild,
      $actual->firstChild,
      true,
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertEqualXMLStructure
   */
  public function testXMLStructureIsSameButDataIsNot() {
    $expected = new DOMDocument();
    $expected->load($this->getFilesDirectory().'structureExpected.xml');

    $actual = new DOMDocument();
    $actual->load(
      $this->getFilesDirectory().'structureIsSameButDataIsNot.xml',
    );

    $this->assertEqualXMLStructure(
      $expected->firstChild,
      $actual->firstChild,
      true,
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertEqualXMLStructure
   */
  public function testXMLStructureAttributesAreSameButValuesAreNot() {
    $expected = new DOMDocument();
    $expected->load($this->getFilesDirectory().'structureExpected.xml');

    $actual = new DOMDocument();
    $actual->load(
      $this->getFilesDirectory().
      'structureAttributesAreSameButValuesAreNot.xml',
    );

    $this->assertEqualXMLStructure(
      $expected->firstChild,
      $actual->firstChild,
      true,
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertEqualXMLStructure
   */
  public function testXMLStructureIgnoreTextNodes() {
    $expected = new DOMDocument();
    $expected->load($this->getFilesDirectory().'structureExpected.xml');

    $actual = new DOMDocument();
    $actual->load($this->getFilesDirectory().'structureIgnoreTextNodes.xml');

    $this->assertEqualXMLStructure(
      $expected->firstChild,
      $actual->firstChild,
      true,
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertEquals
   */
  public function testAssertStringEqualsNumeric() {
    $this->assertEquals('0', 0);

    try {
      $this->assertEquals('0', 1);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotEquals
   */
  public function testAssertStringEqualsNumeric2() {
    $this->assertNotEquals('A', 0);
  }

  // JEO: invalid test function sig string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertFileExists
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertFileExistsThrowsException()
  // {
  //     $this->assertFileExists(null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertFileExists
   */
  public function testAssertFileExists() {
    $this->assertFileExists(__FILE__);

    try {
      $this->assertFileExists(__DIR__.DIRECTORY_SEPARATOR.'NotExisting');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertFileNotExists
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertFileNotExistsThrowsException()
  // {
  //     $this->assertFileNotExists(null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertFileNotExists
   */
  public function testAssertFileNotExists() {
    $this->assertFileNotExists(__DIR__.DIRECTORY_SEPARATOR.'NotExisting');

    try {
      $this->assertFileNotExists(__FILE__);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectHasAttribute
   */
  public function testAssertObjectHasAttribute() {
    $o = new Author('Terry Pratchett');

    $this->assertObjectHasAttribute('name', $o);

    try {
      $this->assertObjectHasAttribute('foo', $o);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectNotHasAttribute
   */
  public function testAssertObjectNotHasAttribute() {
    $o = new Author('Terry Pratchett');

    $this->assertObjectNotHasAttribute('foo', $o);

    try {
      $this->assertObjectNotHasAttribute('name', $o);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertFinite
   */
  public function testAssertFinite() {
    $this->assertFinite(1);

    try {
      $this->assertFinite(INF);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertInfinite
   */
  public function testAssertInfinite() {
    $this->assertInfinite(INF);

    try {
      $this->assertInfinite(1);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNan
   */
  public function testAssertNan() {
    $this->assertNan(NAN);

    try {
      $this->assertNan(1);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNull
   */
  public function testAssertNull() {
    $this->assertNull(null);

    try {
      $this->assertNull(new stdClass());
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotNull
   */
  public function testAssertNotNull() {
    $this->assertNotNull(new stdClass());

    try {
      $this->assertNotNull(null);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertTrue
   */
  public function testAssertTrue() {
    $this->assertTrue(true);

    try {
      $this->assertTrue(false);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotTrue
   */
  public function testAssertNotTrue() {
    $this->assertNotTrue(false);
    $this->assertNotTrue(1);
    $this->assertNotTrue('true');

    try {
      $this->assertNotTrue(true);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertFalse
   */
  public function testAssertFalse() {
    $this->assertFalse(false);

    try {
      $this->assertFalse(true);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotFalse
   */
  public function testAssertNotFalse() {
    $this->assertNotFalse(true);
    $this->assertNotFalse(0);
    $this->assertNotFalse('');

    try {
      $this->assertNotFalse(false);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig, string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertRegExp
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertRegExpThrowsException()
  // {
  //     $this->assertRegExp(null, null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertRegExp
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertRegExpThrowsException2()
  // {
  //     $this->assertRegExp('', null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertNotRegExp
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertNotRegExpThrowsException()
  // {
  //     $this->assertNotRegExp(null, null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertNotRegExp
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertNotRegExpThrowsException2()
  // {
  //     $this->assertNotRegExp('', null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertRegExp
   */
  public function testAssertRegExp() {
    $this->assertRegExp('/foo/', 'foobar');

    try {
      $this->assertRegExp('/foo/', 'bar');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotRegExp
   */
  public function testAssertNotRegExp() {
    $this->assertNotRegExp('/foo/', 'bar');

    try {
      $this->assertNotRegExp('/foo/', 'foobar');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertSame
   */
  public function testAssertSame() {
    $o = new stdClass();

    $this->assertSame($o, $o);

    try {
      $this->assertSame(new stdClass(), new stdClass());
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertSame
   */
  public function testAssertSame2() {
    $this->assertSame(true, true);
    $this->assertSame(false, false);

    try {
      $this->assertSame(true, false);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotSame
   */
  public function testAssertNotSame() {
    $this->assertNotSame(new stdClass(), null);

    $this->assertNotSame(null, new stdClass());

    $this->assertNotSame(new stdClass(), new stdClass());

    $o = new stdClass();

    try {
      $this->assertNotSame($o, $o);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotSame
   */
  public function testAssertNotSame2() {
    $this->assertNotSame(true, false);
    $this->assertNotSame(false, true);

    try {
      $this->assertNotSame(true, true);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotSame
   */
  public function testAssertNotSameFailsNull() {
    try {
      $this->assertNotSame(null, null);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertGreaterThan
   */
  public function testGreaterThan() {
    $this->assertGreaterThan(1, 2);

    try {
      $this->assertGreaterThan(2, 1);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeGreaterThan
   */
  public function testAttributeGreaterThan() {
    $this->assertAttributeGreaterThan(
      1,
      'bar',
      new ClassWithNonPublicAttributes(),
    );

    try {
      $this->assertAttributeGreaterThan(
        1,
        'foo',
        new ClassWithNonPublicAttributes(),
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertGreaterThanOrEqual
   */
  public function testGreaterThanOrEqual() {
    $this->assertGreaterThanOrEqual(1, 2);

    try {
      $this->assertGreaterThanOrEqual(2, 1);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeGreaterThanOrEqual
   */
  public function testAttributeGreaterThanOrEqual() {
    $this->assertAttributeGreaterThanOrEqual(
      1,
      'bar',
      new ClassWithNonPublicAttributes(),
    );

    try {
      $this->assertAttributeGreaterThanOrEqual(
        2,
        'foo',
        new ClassWithNonPublicAttributes(),
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertLessThan
   */
  public function testLessThan() {
    $this->assertLessThan(2, 1);

    try {
      $this->assertLessThan(1, 2);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeLessThan
   */
  public function testAttributeLessThan() {
    $this->assertAttributeLessThan(
      2,
      'foo',
      new ClassWithNonPublicAttributes(),
    );

    try {
      $this->assertAttributeLessThan(
        1,
        'bar',
        new ClassWithNonPublicAttributes(),
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertLessThanOrEqual
   */
  public function testLessThanOrEqual() {
    $this->assertLessThanOrEqual(2, 1);

    try {
      $this->assertLessThanOrEqual(1, 2);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeLessThanOrEqual
   */
  public function testAttributeLessThanOrEqual() {
    $this->assertAttributeLessThanOrEqual(
      2,
      'foo',
      new ClassWithNonPublicAttributes(),
    );

    try {
      $this->assertAttributeLessThanOrEqual(
        1,
        'bar',
        new ClassWithNonPublicAttributes(),
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::readAttribute
   * @covers PHPUnit_Framework_Assert::getStaticAttribute
   * @covers PHPUnit_Framework_Assert::getObjectAttribute
   */
  public function testReadAttribute() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertEquals(
      'foo',
      static::readAttribute($obj, 'publicAttribute'),
    );
    $this->assertEquals(
      'bar',
      static::readAttribute($obj, 'protectedAttribute'),
    );
    $this->assertEquals(
      'baz',
      static::readAttribute($obj, 'privateAttribute'),
    );
    $this->assertEquals(
      'bar',
      static::readAttribute($obj, 'protectedParentAttribute'),
    );
    //$this->assertEquals('bar', static::readAttribute($obj, 'privateParentAttribute'));
  }

  /**
   * @covers PHPUnit_Framework_Assert::readAttribute
   * @covers PHPUnit_Framework_Assert::getStaticAttribute
   * @covers PHPUnit_Framework_Assert::getObjectAttribute
   */
  public function testReadAttribute2() {
    $this->assertEquals(
      'foo',
      static::readAttribute(
        ClassWithNonPublicAttributes::class,
        'publicStaticAttribute',
      ),
    );
    $this->assertEquals(
      'bar',
      static::readAttribute(
        ClassWithNonPublicAttributes::class,
        'protectedStaticAttribute',
      ),
    );
    $this->assertEquals(
      'baz',
      static::readAttribute(
        ClassWithNonPublicAttributes::class,
        'privateStaticAttribute',
      ),
    );
    $this->assertEquals(
      'foo',
      static::readAttribute(
        ClassWithNonPublicAttributes::class,
        'protectedStaticParentAttribute',
      ),
    );
    $this->assertEquals(
      'foo',
      static::readAttribute(
        ClassWithNonPublicAttributes::class,
        'privateStaticParentAttribute',
      ),
    );
  }

  // JEO: invalid test function sig untyped, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::readAttribute
  //  * @covers            PHPUnit_Framework_Assert::getStaticAttribute
  //  * @covers            PHPUnit_Framework_Assert::getObjectAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testReadAttribute3()
  // {
  //     static::readAttribute('StdClass', null);
  // }

  /**
   * @covers            PHPUnit_Framework_Assert::readAttribute
   * @covers            PHPUnit_Framework_Assert::getStaticAttribute
   * @covers            PHPUnit_Framework_Assert::getObjectAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testReadAttribute4() {
    static::readAttribute('NotExistingClass', 'foo');
  }

  /**
   * @covers            PHPUnit_Framework_Assert::readAttribute
   * @covers            PHPUnit_Framework_Assert::getStaticAttribute
   * @covers            PHPUnit_Framework_Assert::getObjectAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testReadAttribute5() {
    static::readAttribute(null, 'foo');
  }

  /**
   * @covers            PHPUnit_Framework_Assert::readAttribute
   * @covers            PHPUnit_Framework_Assert::getStaticAttribute
   * @covers            PHPUnit_Framework_Assert::getObjectAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testReadAttributeIfAttributeNameIsNotValid() {
    static::readAttribute(stdClass::class, '2');
  }

  // JEO: invalid test function sig classname<T>, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::getStaticAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testGetStaticAttributeRaisesExceptionForInvalidFirstArgument()
  // {
  //     static::getStaticAttribute(null, 'foo');
  // }

  // JEO: invalid test, function signature has classname<T> typechecker borks on
  // invalid use.
  // /**
  //  * @covers PHPUnit_Framework_Assert::getStaticAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testGetStaticAttributeRaisesExceptionForInvalidFirstArgument2()
  // {
  //     static::getStaticAttribute('NotExistingClass', 'foo');
  // }

  // JEO: invalid test function sig classname<T>, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::getStaticAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testGetStaticAttributeRaisesExceptionForInvalidSecondArgument()
  // {
  //     static::getStaticAttribute(stdClass::class, null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::getStaticAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testGetStaticAttributeRaisesExceptionForInvalidSecondArgument2(
  ) {
    static::getStaticAttribute(stdClass::class, '0');
  }

  /**
   * @covers PHPUnit_Framework_Assert::getStaticAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testGetStaticAttributeRaisesExceptionForInvalidSecondArgument3(
  ) {
    static::getStaticAttribute(stdClass::class, 'foo');
  }

  /**
   * @covers PHPUnit_Framework_Assert::getObjectAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testGetObjectAttributeRaisesExceptionForInvalidFirstArgument(
  ) {
    static::getObjectAttribute(null, 'foo');
  }

  // JEO: invalid test function sig untyped, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::getObjectAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testGetObjectAttributeRaisesExceptionForInvalidSecondArgument()
  // {
  //     static::getObjectAttribute(new stdClass(), null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::getObjectAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testGetObjectAttributeRaisesExceptionForInvalidSecondArgument2(
  ) {
    static::getObjectAttribute(new stdClass(), '0');
  }

  /**
   * @covers PHPUnit_Framework_Assert::getObjectAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testGetObjectAttributeRaisesExceptionForInvalidSecondArgument3(
  ) {
    static::getObjectAttribute(new stdClass(), 'foo');
  }

  /**
   * @covers PHPUnit_Framework_Assert::getObjectAttribute
   */
  public function testGetObjectAttributeWorksForInheritedAttributes() {
    $this->assertEquals(
      'bar',
      static::getObjectAttribute(
        new ClassWithNonPublicAttributes(),
        'privateParentAttribute',
      ),
    );
  }

  public function testAssertPublicAttributeContains() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeContains('foo', 'publicArray', $obj);

    try {
      $this->assertAttributeContains('bar', 'publicArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail(
      'Failed exception trap, should of raised AssertionFailedException',
    );
  }

  public function testAssertPublicAttributeContainsOnly() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeContainsOnly('string', 'publicArray', $obj);

    try {
      $this->assertAttributeContainsOnly('integer', 'publicArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail(
      'Failed exception trap, should of raised AssertionFailedException',
    );

  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotContains
   */
  public function testAssertPublicAttributeNotContains() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotContains('bar', 'publicArray', $obj);

    try {
      $this->assertAttributeNotContains('foo', 'publicArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail(
      'Failed exception trap, should of raised AssertionFailedException',
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotContainsOnly
   */
  public function testAssertPublicAttributeNotContainsOnly() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotContainsOnly('integer', 'publicArray', $obj);

    try {
      $this->assertAttributeNotContainsOnly('string', 'publicArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertProtectedAttributeContains() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeContains('bar', 'protectedArray', $obj);

    try {
      $this->assertAttributeContains('foo', 'protectedArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertProtectedAttributeNotContains() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotContains('foo', 'protectedArray', $obj);

    try {
      $this->assertAttributeNotContains('bar', 'protectedArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertPrivateAttributeContains() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeContains('baz', 'privateArray', $obj);

    try {
      $this->assertAttributeContains('foo', 'privateArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertPrivateAttributeNotContains() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotContains('foo', 'privateArray', $obj);

    try {
      $this->assertAttributeNotContains('baz', 'privateArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertAttributeContainsNonObject() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeContains(true, 'privateArray', $obj);

    try {
      $this->assertAttributeContains(
        true,
        'privateArray',
        $obj,
        '',
        false,
        true,
        true,
      );
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertAttributeNotContainsNonObject() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotContains(
      true,
      'privateArray',
      $obj,
      '',
      false,
      true,
      true,
    );

    try {
      $this->assertAttributeNotContains(true, 'privateArray', $obj);
    } catch (AssertionFailedException $e) {
      return;
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertPublicAttributeEquals() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeEquals('foo', 'publicAttribute', $obj);

    try {
      $this->assertAttributeEquals('bar', 'publicAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertPublicAttributeNotEquals() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotEquals('bar', 'publicAttribute', $obj);

    try {
      $this->assertAttributeNotEquals('foo', 'publicAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertPublicAttributeSame() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeSame('foo', 'publicAttribute', $obj);

    try {
      $this->assertAttributeSame('bar', 'publicAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertPublicAttributeNotSame() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotSame('bar', 'publicAttribute', $obj);

    try {
      $this->assertAttributeNotSame('foo', 'publicAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertProtectedAttributeEquals() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeEquals('bar', 'protectedAttribute', $obj);

    try {
      $this->assertAttributeEquals('foo', 'protectedAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  public function testAssertProtectedAttributeNotEquals() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotEquals('foo', 'protectedAttribute', $obj);

    try {
      $this->assertAttributeNotEquals('bar', 'protectedAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeEquals
   */
  public function testAssertPrivateAttributeEquals() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeEquals('baz', 'privateAttribute', $obj);

    try {
      $this->assertAttributeEquals('foo', 'privateAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotEquals
   */
  public function testAssertPrivateAttributeNotEquals() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertAttributeNotEquals('foo', 'privateAttribute', $obj);

    try {
      $this->assertAttributeNotEquals('baz', 'privateAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeEquals
   */
  public function testAssertPublicStaticAttributeEquals() {
    $this->assertAttributeEquals(
      'foo',
      'publicStaticAttribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertAttributeEquals(
        'bar',
        'publicStaticAttribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotEquals
   */
  public function testAssertPublicStaticAttributeNotEquals() {
    $this->assertAttributeNotEquals(
      'bar',
      'publicStaticAttribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertAttributeNotEquals(
        'foo',
        'publicStaticAttribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeEquals
   */
  public function testAssertProtectedStaticAttributeEquals() {
    $this->assertAttributeEquals(
      'bar',
      'protectedStaticAttribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertAttributeEquals(
        'foo',
        'protectedStaticAttribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotEquals
   */
  public function testAssertProtectedStaticAttributeNotEquals() {
    $this->assertAttributeNotEquals(
      'foo',
      'protectedStaticAttribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertAttributeNotEquals(
        'bar',
        'protectedStaticAttribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeEquals
   */
  public function testAssertPrivateStaticAttributeEquals() {
    $this->assertAttributeEquals(
      'baz',
      'privateStaticAttribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertAttributeEquals(
        'foo',
        'privateStaticAttribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotEquals
   */
  public function testAssertPrivateStaticAttributeNotEquals() {
    $this->assertAttributeNotEquals(
      'foo',
      'privateStaticAttribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertAttributeNotEquals(
        'baz',
        'privateStaticAttribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertClassHasAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertClassHasAttributeThrowsException()
  // {
  //     $this->assertClassHasAttribute(null, null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertClassHasAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertClassHasAttributeThrowsException2()
  // {
  //     $this->assertClassHasAttribute('foo', null);
  // }

  /**
   * @covers            PHPUnit_Framework_Assert::assertClassHasAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertClassHasAttributeThrowsExceptionIfAttributeNameIsNotValid(
  ) {
    $this->assertClassHasAttribute('1', 'ClassWithNonPublicAttributes');
  }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertClassNotHasAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertClassNotHasAttributeThrowsException()
  // {
  //     $this->assertClassNotHasAttribute(null, null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertClassNotHasAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertClassNotHasAttributeThrowsException2()
  // {
  //     $this->assertClassNotHasAttribute('foo', null);
  // }

  /**
   * @covers            PHPUnit_Framework_Assert::assertClassNotHasAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertClassNotHasAttributeThrowsExceptionIfAttributeNameIsNotValid(
  ) {
    $this->assertClassNotHasAttribute('1', 'ClassWithNonPublicAttributes');
  }

  // JEO: invalid test function sig, string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertClassHasStaticAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertClassHasStaticAttributeThrowsException()
  // {
  //     $this->assertClassHasStaticAttribute(null, null);
  // }

  // JEO: invalid test function sig, string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertClassHasStaticAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertClassHasStaticAttributeThrowsException2()
  // {
  //     $this->assertClassHasStaticAttribute('foo', null);
  // }

  /**
   * @covers            PHPUnit_Framework_Assert::assertClassHasStaticAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertClassHasStaticAttributeThrowsExceptionIfAttributeNameIsNotValid(
  ) {
    $this->assertClassHasStaticAttribute('1', 'ClassWithNonPublicAttributes');
  }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertClassNotHasStaticAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertClassNotHasStaticAttributeThrowsException()
  // {
  //     $this->assertClassNotHasStaticAttribute(null, null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertClassNotHasStaticAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertClassNotHasStaticAttributeThrowsException2()
  // {
  //     $this->assertClassNotHasStaticAttribute('foo', null);
  // }

  /**
   * @covers            PHPUnit_Framework_Assert::assertClassNotHasStaticAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertClassNotHasStaticAttributeThrowsExceptionIfAttributeNameIsNotValid(
  ) {
    $this->assertClassNotHasStaticAttribute(
      '1',
      'ClassWithNonPublicAttributes',
    );
  }

  // JEO: invalid function sig string, mixed, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertObjectHasAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertObjectHasAttributeThrowsException()
  // {
  //     $this->assertObjectHasAttribute(null, null);
  // }

  /**
   * @covers            PHPUnit_Framework_Assert::assertObjectHasAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertObjectHasAttributeThrowsException2() {
    $this->assertObjectHasAttribute('foo', null);
  }

  /**
   * @covers            PHPUnit_Framework_Assert::assertObjectHasAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertObjectHasAttributeThrowsExceptionIfAttributeNameIsNotValid(
  ) {
    $this->assertObjectHasAttribute('1', 'ClassWithNonPublicAttributes');
  }

  // JEO: invalid test function sig string, mixed, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertObjectNotHasAttribute
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertObjectNotHasAttributeThrowsException()
  // {
  //     $this->assertObjectNotHasAttribute(null, null);
  // }

  /**
   * @covers            PHPUnit_Framework_Assert::assertObjectNotHasAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertObjectNotHasAttributeThrowsException2() {
    $this->assertObjectNotHasAttribute('foo', null);
  }

  /**
   * @covers            PHPUnit_Framework_Assert::assertObjectNotHasAttribute
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertObjectNotHasAttributeThrowsExceptionIfAttributeNameIsNotValid(
  ) {
    $this->assertObjectNotHasAttribute('1', 'ClassWithNonPublicAttributes');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertClassHasAttribute
   */
  public function testClassHasPublicAttribute() {
    $this->assertClassHasAttribute(
      'publicAttribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertClassHasAttribute(
        'attribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertClassNotHasAttribute
   */
  public function testClassNotHasPublicAttribute() {
    $this->assertClassNotHasAttribute(
      'attribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertClassNotHasAttribute(
        'publicAttribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertClassHasStaticAttribute
   */
  public function testClassHasPublicStaticAttribute() {
    $this->assertClassHasStaticAttribute(
      'publicStaticAttribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertClassHasStaticAttribute(
        'attribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertClassNotHasStaticAttribute
   */
  public function testClassNotHasPublicStaticAttribute() {
    $this->assertClassNotHasStaticAttribute(
      'attribute',
      ClassWithNonPublicAttributes::class,
    );

    try {
      $this->assertClassNotHasStaticAttribute(
        'publicStaticAttribute',
        ClassWithNonPublicAttributes::class,
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectHasAttribute
   */
  public function testObjectHasPublicAttribute() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertObjectHasAttribute('publicAttribute', $obj);

    try {
      $this->assertObjectHasAttribute('attribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectNotHasAttribute
   */
  public function testObjectNotHasPublicAttribute() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertObjectNotHasAttribute('attribute', $obj);

    try {
      $this->assertObjectNotHasAttribute('publicAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectHasAttribute
   */
  public function testObjectHasOnTheFlyAttribute() {
    $obj = new stdClass();
    $obj->foo = 'bar';

    $this->assertObjectHasAttribute('foo', $obj);

    try {
      $this->assertObjectHasAttribute('bar', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectNotHasAttribute
   */
  public function testObjectNotHasOnTheFlyAttribute() {
    $obj = new stdClass();
    $obj->foo = 'bar';

    $this->assertObjectNotHasAttribute('bar', $obj);

    try {
      $this->assertObjectNotHasAttribute('foo', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectHasAttribute
   */
  public function testObjectHasProtectedAttribute() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertObjectHasAttribute('protectedAttribute', $obj);

    try {
      $this->assertObjectHasAttribute('attribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectNotHasAttribute
   */
  public function testObjectNotHasProtectedAttribute() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertObjectNotHasAttribute('attribute', $obj);

    try {
      $this->assertObjectNotHasAttribute('protectedAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectHasAttribute
   */
  public function testObjectHasPrivateAttribute() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertObjectHasAttribute('privateAttribute', $obj);

    try {
      $this->assertObjectHasAttribute('attribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertObjectNotHasAttribute
   */
  public function testObjectNotHasPrivateAttribute() {
    $obj = new ClassWithNonPublicAttributes();

    $this->assertObjectNotHasAttribute('attribute', $obj);

    try {
      $this->assertObjectNotHasAttribute('privateAttribute', $obj);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::attribute
   * @covers PHPUnit_Framework_Assert::equalTo
   */
  public function testAssertThatAttributeEquals() {
    $this->assertThat(
      new ClassWithNonPublicAttributes(),
      $this->attribute($this->equalTo('foo'), 'publicAttribute'),
    );
  }

  /**
   * @covers            PHPUnit_Framework_Assert::assertThat
   * @covers            PHPUnit_Framework_Assert::attribute
   * @covers            PHPUnit_Framework_Assert::equalTo
   * @expectedException PHPUnit_Framework_AssertionFailedError
   */
  public function testAssertThatAttributeEquals2() {
    $this->assertThat(
      new ClassWithNonPublicAttributes(),
      $this->attribute($this->equalTo('bar'), 'publicAttribute'),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::attribute
   * @covers PHPUnit_Framework_Assert::equalTo
   */
  public function testAssertThatAttributeEqualTo() {
    $this->assertThat(
      new ClassWithNonPublicAttributes(),
      $this->attributeEqualTo('publicAttribute', 'foo'),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::anything
   */
  public function testAssertThatAnything() {
    $this->assertThat('anything', $this->anything());
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::isTrue
   */
  public function testAssertThatIsTrue() {
    $this->assertThat(true, $this->isTrue());
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::isFalse
   */
  public function testAssertThatIsFalse() {
    $this->assertThat(false, $this->isFalse());
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::isJson
   */
  public function testAssertThatIsJson() {
    $this->assertThat('{}', $this->isJson());
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::anything
   * @covers PHPUnit_Framework_Assert::logicalAnd
   */
  public function testAssertThatAnythingAndAnything() {
    $this->assertThat(
      'anything',
      $this->logicalAnd($this->anything(), $this->anything()),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::anything
   * @covers PHPUnit_Framework_Assert::logicalOr
   */
  public function testAssertThatAnythingOrAnything() {
    $this->assertThat(
      'anything',
      $this->logicalOr($this->anything(), $this->anything()),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::anything
   * @covers PHPUnit_Framework_Assert::logicalNot
   * @covers PHPUnit_Framework_Assert::logicalXor
   */
  public function testAssertThatAnythingXorNotAnything() {
    $this->assertThat(
      'anything',
      $this->logicalXor(
        $this->anything(),
        $this->logicalNot($this->anything()),
      ),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::contains
   */
  public function testAssertThatContains() {
    $this->assertThat(['foo'], $this->contains('foo'));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::stringContains
   */
  public function testAssertThatStringContains() {
    $this->assertThat('barfoobar', $this->stringContains('foo'));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::containsOnly
   */
  public function testAssertThatContainsOnly() {
    $this->assertThat(['foo'], $this->containsOnly('string'));
  }
  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::containsOnlyInstancesOf
   */
  public function testAssertThatContainsOnlyInstancesOf() {
    $this->assertThat(
      [new Book()],
      $this->containsOnlyInstancesOf(Book::class),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::arrayHasKey
   */
  public function testAssertThatArrayHasKey() {
    $this->assertThat(['foo' => 'bar'], $this->arrayHasKey('foo'));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::classHasAttribute
   */
  public function testAssertThatClassHasAttribute() {
    $this->assertThat(
      new ClassWithNonPublicAttributes(),
      $this->classHasAttribute('publicAttribute'),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::classHasStaticAttribute
   */
  public function testAssertThatClassHasStaticAttribute() {
    $this->assertThat(
      new ClassWithNonPublicAttributes(),
      $this->classHasStaticAttribute('publicStaticAttribute'),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::objectHasAttribute
   */
  public function testAssertThatObjectHasAttribute() {
    $this->assertThat(
      new ClassWithNonPublicAttributes(),
      $this->objectHasAttribute('publicAttribute'),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::equalTo
   */
  public function testAssertThatEqualTo() {
    $this->assertThat('foo', $this->equalTo('foo'));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::identicalTo
   */
  public function testAssertThatIdenticalTo() {
    $value = new stdClass();
    $constraint = $this->identicalTo($value);

    $this->assertThat($value, $constraint);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::isInstanceOf
   */
  public function testAssertThatIsInstanceOf() {
    $this->assertThat(new stdClass(), $this->isInstanceOf(stdClass::class));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::isType
   */
  public function testAssertThatIsType() {
    $this->assertThat('string', $this->isType('string'));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::isEmpty
   */
  public function testAssertThatIsEmpty() {
    $this->assertThat([], $this->isEmpty());
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::fileExists
   */
  public function testAssertThatFileExists() {
    $this->assertThat(__FILE__, $this->fileExists());
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::greaterThan
   */
  public function testAssertThatGreaterThan() {
    $this->assertThat(2, $this->greaterThan(1));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::greaterThanOrEqual
   */
  public function testAssertThatGreaterThanOrEqual() {
    $this->assertThat(2, $this->greaterThanOrEqual(1));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::lessThan
   */
  public function testAssertThatLessThan() {
    $this->assertThat(1, $this->lessThan(2));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::lessThanOrEqual
   */
  public function testAssertThatLessThanOrEqual() {
    $this->assertThat(1, $this->lessThanOrEqual(2));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::matchesRegularExpression
   */
  public function testAssertThatMatchesRegularExpression() {
    $this->assertThat('foobar', $this->matchesRegularExpression('/foo/'));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::callback
   */
  public function testAssertThatCallback() {
    $this->assertThat(
      null,
      $this->callback(
        function($other) {
          return true;
        },
      ),
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertThat
   * @covers PHPUnit_Framework_Assert::countOf
   */
  public function testAssertThatCountOf() {
    $this->assertThat([1], $this->countOf(1));
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertFileEquals
   */
  public function testAssertFileEquals() {
    $this->assertFileEquals(
      $this->getFilesDirectory().'foo.xml',
      $this->getFilesDirectory().'foo.xml',
    );

    try {
      $this->assertFileEquals(
        $this->getFilesDirectory().'foo.xml',
        $this->getFilesDirectory().'bar.xml',
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertFileNotEquals
   */
  public function testAssertFileNotEquals() {
    $this->assertFileNotEquals(
      $this->getFilesDirectory().'foo.xml',
      $this->getFilesDirectory().'bar.xml',
    );

    try {
      $this->assertFileNotEquals(
        $this->getFilesDirectory().'foo.xml',
        $this->getFilesDirectory().'foo.xml',
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringEqualsFile
   */
  public function testAssertStringEqualsFile() {
    $this->assertStringEqualsFile(
      $this->getFilesDirectory().'foo.xml',
      file_get_contents($this->getFilesDirectory().'foo.xml'),
    );

    try {
      $this->assertStringEqualsFile(
        $this->getFilesDirectory().'foo.xml',
        file_get_contents($this->getFilesDirectory().'bar.xml'),
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringNotEqualsFile
   */
  public function testAssertStringNotEqualsFile() {
    $this->assertStringNotEqualsFile(
      $this->getFilesDirectory().'foo.xml',
      file_get_contents($this->getFilesDirectory().'bar.xml'),
    );

    try {
      $this->assertStringNotEqualsFile(
        $this->getFilesDirectory().'foo.xml',
        file_get_contents($this->getFilesDirectory().'foo.xml'),
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertStringStartsWith
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringStartsWithThrowsException()
  // {
  //     $this->assertStringStartsWith(null, null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertStringStartsWith
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringStartsWithThrowsException2()
  // {
  //     $this->assertStringStartsWith('', null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertStringStartsNotWith
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringStartsNotWithThrowsException()
  // {
  //     $this->assertStringStartsNotWith(null, null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertStringStartsNotWith
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringStartsNotWithThrowsException2()
  // {
  //     $this->assertStringStartsNotWith('', null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertStringEndsWith
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringEndsWithThrowsException()
  // {
  //     $this->assertStringEndsWith(null, null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertStringEndsWith
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringEndsWithThrowsException2()
  // {
  //     $this->assertStringEndsWith('', null);
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertStringEndsNotWith
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringEndsNotWithThrowsException()
  // {
  //     $this->assertStringEndsNotWith(null, null);
  // }

  // JEO: Invalid test function sig string, string, string
  // /**
  //  * @covers            PHPUnit_Framework_Assert::assertStringEndsNotWith
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringEndsNotWithThrowsException2()
  // {
  //     $this->assertStringEndsNotWith('', null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringStartsWith
   */
  public function testAssertStringStartsWith() {
    $this->assertStringStartsWith('prefix', 'prefixfoo');

    try {
      $this->assertStringStartsWith('prefix', 'foo');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringStartsNotWith
   */
  public function testAssertStringStartsNotWith() {
    $this->assertStringStartsNotWith('prefix', 'foo');

    try {
      $this->assertStringStartsNotWith('prefix', 'prefixfoo');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringEndsWith
   */
  public function testAssertStringEndsWith() {
    $this->assertStringEndsWith('suffix', 'foosuffix');

    try {
      $this->assertStringEndsWith('suffix', 'foo');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringEndsNotWith
   */
  public function testAssertStringEndsNotWith() {
    $this->assertStringEndsNotWith('suffix', 'foo');

    try {
      $this->assertStringEndsNotWith('suffix', 'foosuffix');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertStringMatchesFormat
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringMatchesFormatRaisesExceptionForInvalidFirstArgument()
  // {
  //     $this->assertStringMatchesFormat(null, '');
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertStringMatchesFormat
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringMatchesFormatRaisesExceptionForInvalidSecondArgument()
  // {
  //     $this->assertStringMatchesFormat('', null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringMatchesFormat
   */
  public function testAssertStringMatchesFormat() {
    $this->assertStringMatchesFormat('*%s*', '***');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringMatchesFormat
   * @expectedException PHPUnit_Framework_AssertionFailedError
   */
  public function testAssertStringMatchesFormatFailure() {
    $this->assertStringMatchesFormat('*%s*', '**');
  }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertStringNotMatchesFormat
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringNotMatchesFormatRaisesExceptionForInvalidFirstArgument()
  // {
  //     $this->assertStringNotMatchesFormat(null, '');
  // }

  // JEO: invalid test function sig string, string, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertStringNotMatchesFormat
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringNotMatchesFormatRaisesExceptionForInvalidSecondArgument()
  // {
  //     $this->assertStringNotMatchesFormat('', null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringNotMatchesFormat
   */
  public function testAssertStringNotMatchesFormat() {
    $this->assertStringNotMatchesFormat('*%s*', '**');

    try {
      $this->assertStringMatchesFormat('*%s*', '**');
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertEmpty
   */
  public function testAssertEmpty() {
    $this->assertEmpty([]);

    try {
      $this->assertEmpty(['foo']);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotEmpty
   */
  public function testAssertNotEmpty() {
    $this->assertNotEmpty(['foo']);

    try {
      $this->assertNotEmpty([]);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeEmpty
   */
  public function testAssertAttributeEmpty() {
    $o = new stdClass();
    $o->a = [];

    $this->assertAttributeEmpty('a', $o);

    try {
      $o->a = ['b'];
      $this->assertAttributeEmpty('a', $o);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotEmpty
   */
  public function testAssertAttributeNotEmpty() {
    $o = new stdClass();
    $o->a = ['b'];

    $this->assertAttributeNotEmpty('a', $o);

    try {
      $o->a = [];
      $this->assertAttributeNotEmpty('a', $o);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::markTestIncomplete
   */
  public function testMarkTestIncomplete() {
    try {
      $this->markTestIncomplete('incomplete');
    } catch (PHPUnit_Framework_IncompleteTestError $e) {
      $this->assertEquals('incomplete', $e->getMessage());

      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::markTestSkipped
   */
  public function testMarkTestSkipped() {
    try {
      $this->markTestSkipped('skipped');
    } catch (PHPUnit_Framework_SkippedTestError $e) {
      $this->assertEquals('skipped', $e->getMessage());

      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertCount
   */
  public function testAssertCount() {
    $this->assertCount(2, [1, 2]);

    try {
      $this->assertCount(2, [1, 2, 3]);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertCount
   */
  public function testAssertCountTraversable() {
    $this->assertCount(2, new ArrayIterator([1, 2]));

    try {
      $this->assertCount(2, new ArrayIterator([1, 2, 3]));
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig int, mixed, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertCount
  //  */
  // public function testAssertCountThrowsExceptionIfExpectedCountIsNoInteger()
  // {
  //     try {
  //         $this->assertCount('a', []);
  //     } catch (PHPUnit_Framework_Exception $e) {
  //         $this->assertEquals('Argument #1 (No Value) of PHPUnit_Framework_Assert::assertCount() must be a integer', $e->getMessage());
  //
  //         return;
  //     }
  //
  //     $this->fail();
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertCount
   */
  public function testAssertCountThrowsExceptionIfElementIsNotCountable() {
    try {
      $this->assertCount(2, '');
    } catch (PHPUnit_Framework_Exception $e) {
      $this->assertEquals(
        'Argument #2 (No Value) of PHPUnit_Framework_Assert::assertCount() must be a countable or traversable',
        $e->getMessage(),
      );

      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeCount
   */
  public function testAssertAttributeCount() {
    $o = new stdClass();
    $o->a = [];

    $this->assertAttributeCount(0, 'a', $o);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotCount
   */
  public function testAssertNotCount() {
    $this->assertNotCount(2, [1, 2, 3]);

    try {
      $this->assertNotCount(2, [1, 2]);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: Invalid test function sig array, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertNotCount
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertNotCountThrowsExceptionIfExpectedCountIsNoInteger()
  // {
  //     $this->assertNotCount('a', []);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotCount
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertNotCountThrowsExceptionIfElementIsNotCountable() {
    $this->assertNotCount(2, '');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotCount
   */
  public function testAssertAttributeNotCount() {
    $o = new stdClass();
    $o->a = [];

    $this->assertAttributeNotCount(1, 'a', $o);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertSameSize
   */
  public function testAssertSameSize() {
    $this->assertSameSize([1, 2], [3, 4]);

    try {
      $this->assertSameSize([1, 2], [1, 2, 3]);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: Invalid test function sig is array, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertSameSize
  //  */
  // public function testAssertSameSizeThrowsExceptionIfExpectedIsNotCountable()
  // {
  //     try {
  //         $this->assertSameSize('a', []);
  //     } catch (PHPUnit_Framework_Exception $e) {
  //         $this->assertEquals('Argument #1 (No Value) of PHPUnit_Framework_Assert::assertSameSize() must be a countable or traversable', $e->getMessage());
  //
  //         return;
  //     }
  //
  //     $this->fail();
  // }

  // JEO: invalid test function sig is array, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertSameSize
  //  */
  // public function testAssertSameSizeThrowsExceptionIfActualIsNotCountable()
  // {
  //     try {
  //         $this->assertSameSize([], '');
  //     } catch (PHPUnit_Framework_Exception $e) {
  //         $this->assertEquals('Argument #2 (No Value) of PHPUnit_Framework_Assert::assertSameSize() must be a countable or traversable', $e->getMessage());
  //
  //         return;
  //     }
  //
  //     $this->fail();
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotSameSize
   */
  public function testAssertNotSameSize() {
    $this->assertNotSameSize([1, 2], [1, 2, 3]);

    try {
      $this->assertNotSameSize([1, 2], [3, 4]);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test function sig is array, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertNotSameSize
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertNotSameSizeThrowsExceptionIfExpectedIsNotCountable()
  // {
  //     $this->assertNotSameSize('a', []);
  // }

  // JEO: invalid test function sig is array, array, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertNotSameSize
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertNotSameSizeThrowsExceptionIfActualIsNotCountable()
  // {
  //     $this->assertNotSameSize([], '');
  // }

  // JEO: invalid test function sig is string, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertJson
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertJsonRaisesExceptionForInvalidArgument()
  // {
  //     $this->assertJson(null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertJson
   */
  public function testAssertJson() {
    $this->assertJson('{}');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonStringEqualsJsonString
   */
  public function testAssertJsonStringEqualsJsonString() {
    $expected = '{"Mascott" : "Tux"}';
    $actual = '{"Mascott" : "Tux"}';
    $message = 'Given Json strings do not match';

    $this->assertJsonStringEqualsJsonString($expected, $actual, $message);
  }

  /**
   * @dataProvider validInvalidJsonDataprovider
   * @covers PHPUnit_Framework_Assert::assertJsonStringEqualsJsonString
   */
  public function testAssertJsonStringEqualsJsonStringErrorRaised(
    $expected,
    $actual,
  ) {
    try {
      $this->assertJsonStringEqualsJsonString($expected, $actual);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }
    $this->fail('Expected exception not found');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonStringNotEqualsJsonString
   */
  public function testAssertJsonStringNotEqualsJsonString() {
    $expected = '{"Mascott" : "Beastie"}';
    $actual = '{"Mascott" : "Tux"}';
    $message = 'Given Json strings do match';

    $this->assertJsonStringNotEqualsJsonString($expected, $actual, $message);
  }

  /**
   * @dataProvider validInvalidJsonDataprovider
   * @covers PHPUnit_Framework_Assert::assertJsonStringNotEqualsJsonString
   */
  public function testAssertJsonStringNotEqualsJsonStringErrorRaised(
    $expected,
    $actual,
  ) {
    try {
      $this->assertJsonStringNotEqualsJsonString($expected, $actual);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }
    $this->fail('Expected exception not found');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonStringEqualsJsonFile
   */
  public function testAssertJsonStringEqualsJsonFile() {
    $file = $this->getFilesDirectory().'/JsonData/simpleObject.json';
    $actual = json_encode(['Mascott' => 'Tux']);
    $message = '';
    $this->assertJsonStringEqualsJsonFile($file, $actual, $message);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonStringEqualsJsonFile
   */
  public function testAssertJsonStringEqualsJsonFileExpectingExpectationFailedException(
  ) {
    $file = $this->getFilesDirectory().'/JsonData/simpleObject.json';
    $actual = json_encode(['Mascott' => 'Beastie']);
    $message = '';
    try {
      $this->assertJsonStringEqualsJsonFile($file, $actual, $message);
    } catch (PHPUnit_Framework_ExpectationFailedException $e) {
      $this->assertEquals(
        'Failed asserting that \'{"Mascott":"Beastie"}\' matches JSON string "{"Mascott":"Tux"}".',
        $e->getMessage(),
      );

      return;
    }

    $this->fail('Expected Exception not thrown.');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonStringEqualsJsonFile
   */
  public function testAssertJsonStringEqualsJsonFileExpectingException() {
    $file = $this->getFilesDirectory().'/JsonData/simpleObject.json';
    try {
      $this->assertJsonStringEqualsJsonFile($file, '');
    } catch (PHPUnit_Framework_Exception $e) {
      return;
    }
    $this->fail('Expected Exception not thrown.');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonStringNotEqualsJsonFile
   */
  public function testAssertJsonStringNotEqualsJsonFile() {
    $file = $this->getFilesDirectory().'/JsonData/simpleObject.json';
    $actual = json_encode(['Mascott' => 'Beastie']);
    $message = '';
    $this->assertJsonStringNotEqualsJsonFile($file, $actual, $message);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonStringNotEqualsJsonFile
   */
  public function testAssertJsonStringNotEqualsJsonFileExpectingException() {
    $file = $this->getFilesDirectory().'/JsonData/simpleObject.json';
    try {
      $this->assertJsonStringNotEqualsJsonFile($file, '');
    } catch (PHPUnit_Framework_Exception $e) {
      return;
    }
    $this->fail('Expected exception not found.');
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonFileNotEqualsJsonFile
   */
  public function testAssertJsonFileNotEqualsJsonFile() {
    $fileExpected = $this->getFilesDirectory().'/JsonData/simpleObject.json';
    $fileActual = $this->getFilesDirectory().'/JsonData/arrayObject.json';
    $message = '';
    $this->assertJsonFileNotEqualsJsonFile(
      $fileExpected,
      $fileActual,
      $message,
    );
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertJsonFileEqualsJsonFile
   */
  public function testAssertJsonFileEqualsJsonFile() {
    $file = $this->getFilesDirectory().'/JsonData/simpleObject.json';
    $message = '';
    $this->assertJsonFileEqualsJsonFile($file, $file, $message);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertInstanceOf
   */
  public function testAssertInstanceOf() {
    $this->assertInstanceOf(stdClass::class, new stdClass());

    try {
      $this->assertInstanceOf(Exception::class, new stdClass());
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: invalid test as the function signature is classname<T>, mixed, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertInstanceOf
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertInstanceOfThrowsExceptionForInvalidArgument()
  // {
  //     $this->assertInstanceOf(null, new stdClass);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeInstanceOf
   */
  public function testAssertAttributeInstanceOf() {
    $o = new stdClass();
    $o->a = new stdClass();

    $this->assertAttributeInstanceOf(stdClass::class, 'a', $o);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotInstanceOf
   */
  public function testAssertNotInstanceOf() {
    $this->assertNotInstanceOf(Exception::class, new stdClass());

    try {
      $this->assertNotInstanceOf(stdClass::class, new stdClass());
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: This test is invalid as signature is classname<T>, mixed, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertNotInstanceOf
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertNotInstanceOfThrowsExceptionForInvalidArgument()
  // {
  //     $this->assertNotInstanceOf(null, new stdClass);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotInstanceOf
   */
  public function testAssertAttributeNotInstanceOf() {
    $o = new stdClass();
    $o->a = new stdClass();

    // JEO: adjusted this from 'Exception', to Exception::class
    $this->assertAttributeNotInstanceOf(Exception::class, 'a', $o);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertInternalType
   */
  public function testAssertInternalType() {
    $this->assertInternalType('integer', 1);

    try {
      $this->assertInternalType('string', 1);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertInternalType
   */
  public function testAssertInternalTypeDouble() {
    $this->assertInternalType('double', 1.0);

    try {
      $this->assertInternalType('double', 1);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: This test is invalid as the signature is string, mixed, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertInternalType
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertInternalTypeThrowsExceptionForInvalidArgument()
  // {
  //     $this->assertInternalType(null, 1);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeInternalType
   */
  public function testAssertAttributeInternalType() {
    $o = new stdClass();
    $o->a = 1;

    $this->assertAttributeInternalType('integer', 'a', $o);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertNotInternalType
   */
  public function testAssertNotInternalType() {
    $this->assertNotInternalType('string', 1);

    try {
      $this->assertNotInternalType('integer', 1);
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  // JEO: This test is invalid as the signature is string, mixed, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertNotInternalType
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertNotInternalTypeThrowsExceptionForInvalidArgument()
  // {
  //     $this->assertNotInternalType(null, 1);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertAttributeNotInternalType
   */
  public function testAssertAttributeNotInternalType() {
    $o = new stdClass();
    $o->a = 1;

    $this->assertAttributeNotInternalType('string', 'a', $o);
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringMatchesFormatFile
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertStringMatchesFormatFileThrowsExceptionForInvalidArgument(
  ) {
    $this->assertStringMatchesFormatFile('not_existing_file', '');
  }

  // JEO: This test is invalid as the signature due to hhi is string, string, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertStringMatchesFormatFile
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringMatchesFormatFileThrowsExceptionForInvalidArgument2()
  // {
  //     $this->assertStringMatchesFormatFile($this->getFilesDirectory() . 'expectedFileFormat.txt', null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringMatchesFormatFile
   */
  public function testAssertStringMatchesFormatFile() {
    $this->assertStringMatchesFormatFile(
      $this->getFilesDirectory().'expectedFileFormat.txt',
      "FOO\n",
    );

    try {
      $this->assertStringMatchesFormatFile(
        $this->getFilesDirectory().'expectedFileFormat.txt',
        "BAR\n",
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringNotMatchesFormatFile
   * @expectedException PHPUnit_Framework_Exception
   */
  public function testAssertStringNotMatchesFormatFileThrowsExceptionForInvalidArgument(
  ) {
    $this->assertStringNotMatchesFormatFile('not_existing_file', '');
  }

  // JEO: This test is invalidated as the hhi for the function has been changed to string, string, string
  // /**
  //  * @covers PHPUnit_Framework_Assert::assertStringNotMatchesFormatFile
  //  * @expectedException PHPUnit_Framework_Exception
  //  */
  // public function testAssertStringNotMatchesFormatFileThrowsExceptionForInvalidArgument2()
  // {
  //   $this->assertStringNotMatchesFormatFile($this->getFilesDirectory() . 'expectedFileFormat.txt', null);
  // }

  /**
   * @covers PHPUnit_Framework_Assert::assertStringNotMatchesFormatFile
   */
  public function testAssertStringNotMatchesFormatFile() {
    $this->assertStringNotMatchesFormatFile(
      $this->getFilesDirectory().'expectedFileFormat.txt',
      "BAR\n",
    );

    try {
      $this->assertStringNotMatchesFormatFile(
        $this->getFilesDirectory().'expectedFileFormat.txt',
        "FOO\n",
      );
    } catch (PHPUnit_Framework_AssertionFailedError $e) {
      return;
    }

    $this->fail();
  }

  /**
   * @return array
   */
  public static function validInvalidJsonDataprovider() {
    return [
      'error syntax in expected JSON' => [
        '{"Mascott"::}',
        '{"Mascott" : "Tux"}',
      ],
      'error UTF-8 in actual JSON' => [
        '{"Mascott" : "Tux"}',
        '{"Mascott" : :}',
      ],
    ];
  }
}
