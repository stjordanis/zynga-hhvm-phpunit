<?hh // strict

namespace PHPUnit\Tests\Mock;

class SampleClass {
  public int $a;
  protected int $b;
  protected int $c;

  public function __construct(int $a, int $b, int $c) {
    $this->a = $a;
    $this->b = $b;
    $this->c = $c;
  }
}
