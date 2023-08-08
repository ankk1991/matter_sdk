/*
 *
 *    Copyright (c) 2023 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
package chip.devicecontroller.cluster.structs

import chip.devicecontroller.cluster.*
import chip.tlv.AnonymousTag
import chip.tlv.ContextSpecificTag
import chip.tlv.Tag
import chip.tlv.TlvReader
import chip.tlv.TlvWriter

class DishwasherModeClusterModeOptionStruct(
  val label: String,
  val mode: Int,
  val modeTags: List<DishwasherModeClusterModeTagStruct>
) {
  override fun toString(): String = buildString {
    append("DishwasherModeClusterModeOptionStruct {\n")
    append("\tlabel : $label\n")
    append("\tmode : $mode\n")
    append("\tmodeTags : $modeTags\n")
    append("}\n")
  }

  fun toTlv(tag: Tag, tlvWriter: TlvWriter) {
    tlvWriter.apply {
      startStructure(tag)
      put(ContextSpecificTag(TAG_LABEL), label)
      put(ContextSpecificTag(TAG_MODE), mode)
      startList(ContextSpecificTag(TAG_MODE_TAGS))
      for (item in modeTags.iterator()) {
        item.toTlv(AnonymousTag, this)
      }
      endList()
      endStructure()
    }
  }

  companion object {
    private const val TAG_LABEL = 0
    private const val TAG_MODE = 1
    private const val TAG_MODE_TAGS = 2

    fun fromTlv(tag: Tag, tlvReader: TlvReader): DishwasherModeClusterModeOptionStruct {
      tlvReader.enterStructure(tag)
      val label = tlvReader.getString(ContextSpecificTag(TAG_LABEL))
      val mode = tlvReader.getInt(ContextSpecificTag(TAG_MODE))
      val modeTags =
        buildList<DishwasherModeClusterModeTagStruct> {
          tlvReader.enterList(ContextSpecificTag(TAG_MODE_TAGS))
          while (!tlvReader.isEndOfContainer()) {
            add(DishwasherModeClusterModeTagStruct.fromTlv(AnonymousTag, tlvReader))
          }
          tlvReader.exitContainer()
        }

      tlvReader.exitContainer()

      return DishwasherModeClusterModeOptionStruct(label, mode, modeTags)
    }
  }
}
