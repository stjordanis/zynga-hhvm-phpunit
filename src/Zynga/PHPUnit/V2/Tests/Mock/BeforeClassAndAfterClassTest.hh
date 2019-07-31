<?hh // strict

namespace Zynga\PHPUnit\V2\Tests\Mock;

use Zynga\PHPUnit\V2\TestCase;

class BeforeClassAndAfterClassTest extends TestCase {
  public static int $beforeClassWasRun = 0;
  public static int $afterClassWasRun = 0;

  public static function resetProperties(): void {
    self::$beforeClassWasRun = 0;
    self::$afterClassWasRun = 0;
  }

  <<beforeClass()>>
  public static function initialClassSetup(): void {
    self::$beforeClassWasRun++;
  }

  <<afterClass()>>
  public static function finalClassTeardown(): void {
    self::$afterClassWasRun++;
  }

  public function test1(): void {
    $this->assertTrue(true);
  }

  public function test2(): void {
    $this->assertTrue(true);
  }

}
