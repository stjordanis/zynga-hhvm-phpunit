<?hh // strict

namespace SebastianBergmann\CodeCoverage\ProcessedFile\ProcessedFile;

use SebastianBergmann\CodeCoverage\ProcessedFile\ProcessedFile;

class Stats {
  private bool $statsCalculated;
  private int $numExecutableLines;
  private int $numExecutedLines;
  private int $numTraits;
  private int $numTestedTraits;
  private int $numClasses;
  private int $numTestedClasses;
  private int $numMethods;
  private int $numTestedMethods;
  private int $numFunctions;
  private int $numTestedFunctions;
  private ProcessedFile $parent;

  public function __construct(ProcessedFile $parent) {
    $this->statsCalculated = false;
    $this->numExecutableLines = -1;
    $this->numExecutedLines = -1;
    $this->numTraits = -1;
    $this->numTestedTraits = -1;
    $this->numClasses = -1;
    $this->numTestedClasses = -1;
    $this->numMethods = -1;
    $this->numTestedMethods = -1;
    $this->numFunctions = -1;
    $this->numTestedFunctions = -1;
    $this->parent = $parent;
  }

  public function getNumExecutedableLines(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numExecutableLines;
  }

  public function getNumExecutedLines(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numExecutedLines;
  }

  public function getNumTraits(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numTraits;
  }

  public function getNumTestedTraits(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numTestedTraits;
  }

  public function getNumClasses(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numClasses;
  }

  public function getNumTestedClasses(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numTestedClasses;
  }

  public function getNumMethods(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numMethods;
  }

  public function getNumTestedMethods(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numTestedMethods;
  }

  public function getNumFunctions(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numFunctions;
  }

  public function getNumTestedFunctions(bool $recalculate = false): int {
    $this->calculateStatistics($recalculate);
    return $this->numTestedFunctions;
  }

  public function calculateStatistics(bool $recalculate = false): void {

    // if we have already calculated the stats, don't bother running them again.
    if ($this->statsCalculated === true && $recalculate === false) {
      return;
    }

    $this->numExecutableLines = 0;
    $this->numExecutedLines = 0;

    $this->numTraits = 0;
    $this->numTestedTraits = 0;

    $this->numClasses = 0;
    $this->numTestedClasses = 0;

    $this->numMethods = 0;
    $this->numTestedMethods = 0;

    $this->numFunctions = 0;
    $this->numTestedFunctions = 0;

    $stream = $this->parent->stream();

    foreach ($stream->getClasses() as $className => $classObj) {

      $this->numClasses++;
      $this->numExecutableLines += $classObj->executableLines;
      $this->numExecutedLines += $classObj->executedLines;

      $classObj->numMethods = 0;
      $classObj->numTestedMethods = 0;

      $classObj->calculateCoverage();

      foreach ($classObj->methods as $methodObj) {

        $this->numMethods++;
        $classObj->numMethods++;

        // this is a fully covered function
        if ($methodObj->coverage == 100) {
          $this->numTestedMethods++;
          $classObj->numTestedMethods++;
        }

      }

      if ($classObj->coverage == 100) {
        $this->numTestedClasses++;
      }

    }

    foreach ($stream->getTraits() as $traitName => $traitObj) {

      $this->numTraits++;
      $this->numExecutableLines += $traitObj->executableLines;
      $this->numExecutedLines += $traitObj->executedLines;

      $traitObj->numMethods = 0;
      $traitObj->numTestedMethods = 0;

      $traitObj->calculateCoverage();

      foreach ($traitObj->methods as $methodObj) {

        $this->numMethods++;
        $traitObj->numMethods++;

        // this is a fully covered function
        if ($methodObj->coverage == 100) {
          $this->numTestedMethods++;
          $traitObj->numTestedMethods++;
        }

      }

      if ($traitObj->coverage == 100) {
        $this->numTestedClasses++;
      }

    }

    foreach ($stream->getFunctions() as $functionName => $functionObj) {
      $this->numFunctions++;

      // Calculate coverage amount
      $functionObj->calculateCoverage();

      // this is a fully covered function
      if ($functionObj->coverage == 100) {
        $this->numTestedFunctions++;
      }

      $this->numExecutableLines += $functionObj->executableLines;
      $this->numExecutedLines += $functionObj->executedLines;

    }

    $this->statsCalculated = true;

  }

}
