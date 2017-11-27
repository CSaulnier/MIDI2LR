#pragma once
/*
  ==============================================================================

  CommandTableModel.h

This file is part of MIDI2LR. Copyright 2015 by Rory Jaffe.

MIDI2LR is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

MIDI2LR is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
MIDI2LR.  If not, see <http://www.gnu.org/licenses/>.
  ==============================================================================
*/
#ifndef MIDI2LR_COMMANDTABLEMODEL_H
#define MIDI2LR_COMMANDTABLEMODEL_H

#include <utility>
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
class CommandMap;
namespace RSJ {
    enum class MsgIdEnum: short;
    struct MidiMessageId;
}

class CommandTableModel final: public juce::TableListBoxModel {
public:
    CommandTableModel() noexcept;
    void Init(CommandMap* const mapCommand) noexcept;
    CommandTableModel& operator=(const CommandTableModel&) = delete;
    CommandTableModel(const CommandTableModel&) = delete;
    // TableListBoxModel overrides
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    int getNumRows() override;
    void paintRowBackground(juce::Graphics&, int rowNumber, int width,
        int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width,
        int height, bool rowIsSelected) override;
    juce::Component *refreshComponentForCell(int rowNumber, int columnId,
        bool isRowSelected, juce::Component *existingComponentToUpdate) override;

    // adds a row with a corresponding MIDI message to the table
    void addRow(int midi_channel, int midi_data, RSJ::MsgIdEnum msgType);

    // removes a row from the table
    void removeRow(size_t row);

    // removes all rows from the table
    void removeAllRows();

    // builds the table from an XML file
    void buildFromXml(const juce::XmlElement * const elem);

    // returns the index of the row associated to a particular MIDI message
    int getRowForMessage(int midi_channel, int midi_data, RSJ::MsgIdEnum msgType) const;

private:
    void Sort();
    CommandMap* command_map_{nullptr};
    std::pair<int, bool> current_sort{2, true};
    std::pair<int, bool> prior_sort{2, true};
    std::vector<RSJ::MidiMessageId> commands_;
};

#endif