<?hh // strict

namespace SebastianBergmann\TokenStream\Tests;

/*
 * This file is part of php-token-stream.
 *
 * (c) Sebastian Bergmann <sebastian@phpunit.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Environment\CodePath\V1\CodePath;
use SebastianBergmann\TokenStream\Token\Stream;
use SebastianBergmann\TokenStream\Tokens\PHP_Token_Namespace;

class PHP_Token_NamespaceTest extends TestCase {

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
      'token-stream'.
      DIRECTORY_SEPARATOR.
      '_fixture'.
      DIRECTORY_SEPARATOR;
  }

  public function testGetName(): void {
    $tokenStream =
      new Stream($this->getFilesDirectory().'classInNamespace.php');

    $tokens = $tokenStream->tokens();

    foreach ($tokens as $token) {
      if ($token instanceof PHP_Token_Namespace) {
        $this->assertSame('Foo\\Bar', $token->getName());
      }
    }
  }

  public function testGetStartLineWithUnscopedNamespace(): void {
    $tokenStream =
      new Stream($this->getFilesDirectory().'classInNamespace.php');

    $tokens = $tokenStream->tokens();

    foreach ($tokens as $token) {
      if ($token instanceof PHP_Token_Namespace) {
        $this->assertSame(2, $token->getLine());
      }
    }
  }

  public function testGetEndLineWithUnscopedNamespace(): void {
    $tokenStream =
      new Stream($this->getFilesDirectory().'classInNamespace.php');

    $tokens = $tokenStream->tokens();
    foreach ($tokens as $token) {
      if ($token instanceof PHP_Token_Namespace) {
        $this->assertSame(2, $token->getEndLine());
      }
    }
  }

  public function testGetStartLineWithScopedNamespace(): void {
    $tokenStream =
      new Stream($this->getFilesDirectory().'classInScopedNamespace.php');

    $tokens = $tokenStream->tokens();

    foreach ($tokens as $token) {
      if ($token instanceof PHP_Token_Namespace) {
        $this->assertSame(2, $token->getLine());
      }
    }
  }

  public function testGetEndLineWithScopedNamespace(): void {
    $tokenStream =
      new Stream($this->getFilesDirectory().'classInScopedNamespace.php');

    $tokens = $tokenStream->tokens();

    foreach ($tokens as $token) {
      if ($token instanceof PHP_Token_Namespace) {
        $this->assertSame(8, $token->getEndLine());
      }
    }
  }
}
