// Copyright 2019-2023 Cambridge Quantum Computing
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "tket/Circuit/Circuit.hpp"
#include "tket/Utils/Json.hpp"
#include "tket/Utils/PauliStrings.hpp"

namespace tket {

/**
 * MeasurementSetup
 * Captures an expectation value experiment.
 * Given a collection of Pauli terms, the setup contains a collection
 * of measurement circuits from which each Pauli measurement can be extracted.
 *
 * The result_map maps Pauli terms to circuits from which the expectation value
 * can be obtained, along with the classical post-processing required on each
 * shot (which bits/columns of the shot table to XOR together, and whether to
 * invert the final value).
 *
 * There may be multiple measurement circuits that measure the same Pauli term,
 * so we have a vector of MeasurementBitMaps for each term to exploit this and
 * effectively get extra shots for free.
 */
class MeasurementSetup {
 public:
  struct MeasurementBitMap {
    MeasurementBitMap() : circ_index{0}, invert{false} {}
    MeasurementBitMap(
        unsigned _circ_index, const std::vector<unsigned> &_bits,
        bool _invert = 0)
        : circ_index{_circ_index}, bits{_bits}, invert{_invert} {}

    unsigned circ_index;
    std::vector<unsigned> bits;
    bool invert;

    const unsigned &get_circ_index() const { return circ_index; }
    const std::vector<unsigned> &get_bits() const { return bits; }
    const bool &get_invert() const { return invert; }

    std::string to_str() const;
  };
  struct QPSHasher {
    std::size_t operator()(const QubitPauliString &qps) const {
      return hash_value(qps);
    }
  };
  typedef std::unordered_map<
      QubitPauliString, std::vector<MeasurementBitMap>, QPSHasher>
      measure_result_map_t;

  const std::vector<Circuit> &get_circs() const { return measurement_circs; }
  const measure_result_map_t &get_result_map() const { return result_map; }

  void add_measurement_circuit(const Circuit &circ);
  void add_result_for_term(
      const QubitPauliString &term, const MeasurementBitMap &result);
  /**
   * Rejects the coefficient in the QubitPauliTensor. Used for
   * convenience at the C++ level, and should not be exposed.
   */
  void add_result_for_term(
      const QubitPauliTensor &term, const MeasurementBitMap &result);
  /**
   * Checks that the tensors to be measured correspond to the
   * correct tensors generated by the measurement circs. Includes
   * parity checks.
   */
  bool verify() const;

  std::string to_str() const;

 private:
  /**
   * Clifford circuits to generate the correct Pauli measurements,
   * with Measure gates on the required qubits.
   */
  std::vector<Circuit> measurement_circs;
  measure_result_map_t result_map;
};

JSON_DECL(MeasurementSetup::MeasurementBitMap)
JSON_DECL(MeasurementSetup)

}  // namespace tket
