#include "FolderSelector.h"
#include "../Utilities/DvaInfo.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
FolderSelector::FolderSelector(DvaManagerProcessor& processor_ref)
    : folder_button_(FolderButton::TEXT_OVER)
    , processor_ref_(processor_ref)
{
    addAndMakeVisible(&folder_button_);
    folder_button_.onClick = [this]() { openButtonClicked(); };
}

/*---------------------------------------------------------------------------
**
*/
void
FolderSelector::paint(juce::Graphics& g)
{
    auto bounds       = getLocalBounds();
    auto bounds_width = bounds.getWidth();
    auto padding      = 36;

    // Background.
    g.fillAll(TailwindColours::Gray800);

    // Text.
    g.setColour(TailwindColours::Amber50);

    auto       font_height = 18.f;
    juce::Font local_font(font_height, juce::Font::bold);
    g.setFont(local_font);

    g.drawFittedText(
        "Select the folder containing your DVA files.\n\n(note: you can select a directory manually or drag and drop a folder anywhere on this window)",
        0,
        bounds.getCentreY() * 0.5,
        bounds_width,
        font_height,
        juce::Justification::centred,
        1);

    g.drawFittedText(status_label_,
                     0,
                     folder_button_.getBounds().getBottom() + padding,
                     bounds.getWidth(),
                     font_height,
                     juce::Justification::centred,
                     1);
}

/*---------------------------------------------------------------------------
**
*/
void
FolderSelector::resized()
{
    auto bounds        = getLocalBounds();
    auto bounds_width  = bounds.getWidth();
    auto bounds_height = bounds.getHeight();
    auto c_x           = bounds.getCentreX();
    auto c_y           = bounds.getCentreY();
    auto rect_width    = bounds_width * 0.25;
    auto rect_height   = bounds_height * 0.3;

    juce::Rectangle< int > folder_button_container(c_x - (rect_width * 0.5),
                                                   c_y - (rect_height * 0.5),
                                                   rect_width,
                                                   rect_height);

    folder_button_.setBounds(folder_button_container);
}

/*---------------------------------------------------------------------------
**
*/
bool
FolderSelector::isInterestedInFileDrag(const juce::StringArray& files)
{
    if (files.isEmpty()) {
        return false;
    }

    // We're checking one file at a time, not the directory as a whole.
    // Not every file in the directory will be an audio file.
    // We don't want to throw out the entire collection.
    return files[0].toLowerCase().contains(".wav");
}

/*---------------------------------------------------------------------------
**
*/
void
FolderSelector::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::ignoreUnused(x, y);

    if (files.isEmpty()) {
        return;
    }

    for (int i = 0; i < files.size(); ++i) {
        juce::StringArray temp(files[i]);

        if (!isInterestedInFileDrag(temp)) {
            continue;
        }

        auto path = files[i];

        updateState("dva_" + juce::String(i + 1), path);
    }

    status_label_ = "Files successfully loaded.";
    repaint();
}

/*---------------------------------------------------------------------------
**
*/
void
FolderSelector::openButtonClicked()
{
    file_chooser_ = std::make_unique< juce::FileChooser >("Select a folder containing DVA files...", juce::File {});
    auto chooser_flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;

    file_chooser_->launchAsync(chooser_flags, [this](const juce::FileChooser& fc) {
        auto dir        = fc.getResult();
        auto file_paths = dir.findChildFiles(juce::File::findFiles, false, "*.wav");

        file_paths.sort();

        for (int i = 0; i < file_paths.size(); ++i) {
            updateState("dva_" + juce::String(i + 1), file_paths[i].getFullPathName());
        }

        status_label_ = "Files successfully loaded.";
        repaint();
    });
}

/*---------------------------------------------------------------------------
**
*/
void
FolderSelector::updateState(const juce::String& dva_name, const juce::String& file_path)
{
    if (dva_name.isEmpty() || file_path.isEmpty()) {
        return;
    }

    juce::ValueTree dva = processor_ref_.state_.getChildWithName(dva_name);

    if (dva.hasProperty(DvaInfo::getXmlProperty(DvaInfo::PATH))) {
        dva.setProperty(DvaInfo::getXmlProperty(DvaInfo::PATH), file_path, nullptr);
    }
}

/*---------------------------------------------------------------------------
** End of File
*/
