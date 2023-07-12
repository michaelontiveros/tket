# Copyright 2019-2023 Cambridge Quantum Computing
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from typing import Counter
from pytket.circuit import Circuit, OpType  # type: ignore


def gate_counts(circ: Circuit) -> Counter[OpType]:
    """
    Count the number of gates of each type in a circuit.

    :param circ: circuit
    :return: count of gates of each type
    """
    return Counter(cmd.op.type for cmd in circ)