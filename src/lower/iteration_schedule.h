#ifndef TACO_ITERATION_SCHEDULE_H
#define TACO_ITERATION_SCHEDULE_H

#include <memory>
#include <vector>

namespace taco {
class Var;
class Expr;

namespace internal {
class Tensor;
}

namespace lower {
class TensorPath;
class MergeRule;
class MergeLatticePoint;

/// An iteration schedule is a set of index variables arranged as a forest,
/// a set of tensor paths super-imposed on the forest, and a set of merge rules
/// associated with the index variables.
/// - The iteration schedule is arranged in a forest decomposition where all
///   tensor paths move from index variables higher in the tree to index
///   variables strictly lower in the tree.
/// - The tensor paths describe how to iterate over the index variables through
///   the indices of the corresponding (sparse or dense) tensors.
/// - The merge rules describe how to merge the iteration space of the tensor
///   paths incoming on each index variable.
class IterationSchedule {
public:
  IterationSchedule();

  /// Creates an iteration schedule for a tensor with a defined expression.
  static IterationSchedule make(const internal::Tensor&);

  /// Returns the tensor the iteration schedule was built from.
  const internal::Tensor& getTensor() const;

  /// Returns the iteration schedule roots; the index variables with no parents.
  const std::vector<taco::Var>&  getRoots() const;

  /// Returns the children of the index variable
  const std::vector<taco::Var>& getChildren(const taco::Var&) const;

  /// Returns true if the index variable is the ancestor of any free variable.
  bool hasFreeVariableDescendant(const taco::Var&) const;

  /// Returns the number of layers in the iteration schedule. Layers correspond
  /// to loop nests in the emitted code.
  /// @deprecated
  size_t numLayers() const;

  /// Returns a two dimensional ordering of index variables. The first (x)
  /// dimension corresponds to nested loops and the second (y) dimension
  /// correspond to sequenced loops.
  /// @deprecated
  const std::vector<std::vector<taco::Var>>& getLayers() const;

  /// Returns the merge rule of the given var.
  const MergeRule& getMergeRule(const taco::Var&) const;

  /// Returns the tensor paths of the operand tensors in the iteration schedule.
  const std::vector<TensorPath>& getTensorPaths() const;

  /// Returns the tensor path corresponding to a tensor read expression operand.
  const TensorPath& getTensorPath(const taco::Expr& operand) const;

  /// Returns the tensor path of the result tensor.
  const TensorPath& getResultTensorPath() const;

  friend std::ostream& operator<<(std::ostream&, const IterationSchedule&);

private:
  struct Content;
  std::shared_ptr<Content> content;
};

}}
#endif
