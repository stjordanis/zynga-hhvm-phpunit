<?hh // strict

/*
 * This file is part of PHPUnit.
 *
 * (c) Sebastian Bergmann <sebastian@phpunit.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace PHPUnit\Exceptions\Error;

use PHPUnit\Exceptions\Error as BaseError;

/**
 * Wrapper for PHP warnings.
 * You can disable notice-to-exception conversion by setting
 *
 * <code>
 * Warning::$enabled = false;
 * </code>
 *
 * @since Class available since Release 3.3.0
 */
class Warning extends BaseError {
    public static bool $enabled = true;
}