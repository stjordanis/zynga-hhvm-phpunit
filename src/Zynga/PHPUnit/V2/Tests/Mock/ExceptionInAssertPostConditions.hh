<?hh

namespace Zynga\PHPUnit\V2\Tests\Mock;

use Zynga\PHPUnit\V2\TestCase;
use \Exception;

class ExceptionInAssertPostConditions extends TestCase {
  public $setUp = false;
  public $assertPreConditions = false;
  public $assertPostConditions = false;
  public $tearDown = false;
  public $testSomething = false;

  public function setUp() {
    $this->setUp = true;
  }

  public function assertPreConditions() {
    $this->assertPreConditions = true;
  }

  public function testSomething() {
    $this->testSomething = true;
  }

  public function assertPostConditions() {
    $this->assertPostConditions = true;
    throw new Exception();
  }

  public function tearDown() {
    $this->tearDown = true;
  }

}
