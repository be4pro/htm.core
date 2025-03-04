/* ---------------------------------------------------------------------
 * HTM Community Edition of NuPIC
 * Copyright (C) 2013, Numenta, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 *
 * You should have received a copy of the GNU Affero Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 * --------------------------------------------------------------------- */

/** @file
Implementation of Spec API
*/

#include <htm/engine/Spec.hpp>
#include <htm/ntypes/BasicType.hpp>
#include <htm/utils/Log.hpp>

namespace htm {

Spec::Spec() : singleNodeOnly(false), description("") {}
bool Spec::operator==(const Spec &o) const {
  if (singleNodeOnly != o.singleNodeOnly || description != o.description ||
      parameters != o.parameters || outputs != o.outputs ||
      inputs != o.inputs || commands != o.commands) {
    return false;
  }
  return true;
}
std::string Spec::getDefaultInputName() const {
  if (inputs.getCount() == 0)
    return "";
  if (inputs.getCount() == 1)
    return inputs.getByIndex(0).first;

  // search for default input, but detect multple defaults
  bool found = false;
  std::string name;

  for (size_t i = 0; i < inputs.getCount(); ++i) {
    const std::pair<std::string, InputSpec> &p = inputs.getByIndex(i);
    if (p.second.isDefaultInput) {
      NTA_CHECK(!found)
          << "Internal error -- multiply-defined default inputs in Spec";
      found = true;
      name = p.first;
    }
  }
  NTA_CHECK(found)
      << "Internal error -- multiple inputs in Spec but no default";
  return name;
}

std::string Spec::getDefaultOutputName() const {
  if (outputs.getCount() == 0)
    return "";
  if (outputs.getCount() == 1)
    return outputs.getByIndex(0).first;

  // search for default output, but detect multple defaults
  bool found = false;
  std::string name;

  for (size_t i = 0; i < outputs.getCount(); ++i) {
    const std::pair<std::string, OutputSpec> &p = outputs.getByIndex(i);
    if (p.second.isDefaultOutput) {
      NTA_CHECK(!found)
          << "Internal error -- multiply-defined default outputs in Spec";
      found = true;
      name = p.first;
    }
  }
  NTA_CHECK(found)
      << "Internal error -- multiple outputs in Spec but no default";
  return name;
}

InputSpec::InputSpec(std::string description,
                     NTA_BasicType dataType,
                     UInt32 count,
                     bool required,
                     bool regionLevel,
                     bool isDefaultInput)
    : description(std::move(description)), dataType(dataType), count(count),
      required(required), regionLevel(regionLevel),
      isDefaultInput(isDefaultInput) {}

bool InputSpec::operator==(const InputSpec &o) const {
  return required == o.required && regionLevel == o.regionLevel &&
         isDefaultInput == o.isDefaultInput &&
         dataType == o.dataType &&
         count == o.count && description == o.description;
}
std::ostream& operator<< (std::ostream& out, const InputSpec& self) {
   out << "     description: " << self.description << "\n"
       << "     type: " << BasicType::getName(self.dataType) << "\n"
       << "     count: " << self.count << "\n"
       << "     required: " << self.required << "\n"
       << "     regionLevel: " << self.regionLevel << "\n"
       << "     isDefaultInput: " << self.isDefaultInput << "\n";
   return out;
}

OutputSpec::OutputSpec(std::string description,
                       NTA_BasicType dataType,
                       size_t count,
                       bool regionLevel,
                       bool isDefaultOutput)
    : description(std::move(description)), dataType(dataType), count(count),
      regionLevel(regionLevel), isDefaultOutput(isDefaultOutput) {}

bool OutputSpec::operator==(const OutputSpec &o) const {
  return regionLevel == o.regionLevel && isDefaultOutput == o.isDefaultOutput &&
         dataType == o.dataType && count == o.count &&
         description == o.description;
}
std::ostream& operator<< (std::ostream& out, const OutputSpec& self) {
   out << "     description: " << self.description << "\n"
       << "     type: " << BasicType::getName(self.dataType) << "\n"
       << "     count: " << self.count << "\n"
       << "     regionLevel: " << self.regionLevel << "\n"
       << "     isDefaultInput: " << self.isDefaultOutput << "\n";
   return out;
}

CommandSpec::CommandSpec(std::string description)
    : description(std::move(description)) {}
bool CommandSpec::operator==(const CommandSpec &o) const {
  return description == o.description;
}

ParameterSpec::ParameterSpec(std::string description, NTA_BasicType dataType,
                             size_t count, std::string constraints,
                             std::string defaultValue, AccessMode accessMode)
    : description(std::move(description)), dataType(dataType), count(count),
      constraints(std::move(constraints)),
      defaultValue(std::move(defaultValue)), accessMode(accessMode) {
  // Parameter of type byte is not supported;
  // Strings are specified as type byte, length = 0
  if (dataType == NTA_BasicType_Byte && count > 0)
    NTA_THROW << "Parameters of type 'byte' are not supported";
}
bool ParameterSpec::operator==(const ParameterSpec &o) const {
  return dataType == o.dataType && count == o.count &&
         description == o.description && constraints == o.constraints &&
         defaultValue == o.defaultValue && accessMode == o.accessMode;
}
std::ostream& operator<< (std::ostream& out, const ParameterSpec& self) {
    out << "     description: " << self.description << "\n"
        << "     type: " << BasicType::getName(self.dataType) << "\n"
        << "     count: " << self.count << "\n"
        << "     access: ";
    switch(self.accessMode) {
    case ParameterSpec::CreateAccess:  out << "CreateAccess\n"; break;
    case ParameterSpec::ReadOnlyAccess: out << "ReadOnlyAccess\n"; break;
    case ParameterSpec::ReadWriteAccess: out << "ReadWriteAccess\n"; break;
    default: out << "UnknownAccess\n"; break;
    }
    if (!self.defaultValue.empty())
       out << "     defaultValue: " << self.defaultValue << "\n";
    if (!self.constraints.empty())
       out << "     constraints: " << self.constraints << "\n";
    return out;
}

std::string Spec::toString() const {
  // TODO -- minimal information here; fill out with the rest of
  // the parameter spec information
  std::stringstream ss;
  ss << "Spec:"
     << "\n";
  ss << "Description:"
     << "\n"
     << this->description << "\n"
     << "\n";

  ss << "Parameters:"
     << "\n";
  for (size_t i = 0; i < parameters.getCount(); ++i) {
    const std::pair<std::string, ParameterSpec> &item = parameters.getByIndex(i);
    ss << "  " << item.first << "\n";
    ss << item.second << "\n";
  }

  ss << "Inputs:"
     << "\n";
  for (size_t i = 0; i < inputs.getCount(); ++i) {
    const std::pair<std::string, InputSpec> &item = inputs.getByIndex(i);
    ss << "  " << item.first << "\n";
    ss << item.second << "\n";
  }

  ss << "Outputs:"
     << "\n";
  for (size_t i = 0; i < outputs.getCount(); ++i) {
    const std::pair<std::string, OutputSpec> &item = outputs.getByIndex(i);
    ss << "  " << item.first << "\n";
    ss << item.second << "\n";
  }

  if (commands.getCount() > 0) {
    ss << "Commands:"
       << "\n";
    for (size_t i = 0; i < commands.getCount(); ++i) {
      ss << "  " << commands.getByIndex(i).first << ": "
         << commands.getByIndex(i).second.description << "\n";
    }
  }
  return ss.str();
}

std::ostream& operator<< (std::ostream& out, const Spec& self) {
  out << self.toString() << std::endl;
  return out;
}


} // namespace htm
