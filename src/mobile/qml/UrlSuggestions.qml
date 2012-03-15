import QtQuick 2.0
import "UiConstants.js" as UiConstants

Item {
    id: urlSuggestions
    signal suggestionSelected(string suggestedUrl)
    signal searchSelected()

    // FIXME: implement this model in C++ to provide valid and related data (i.e using history)
    ListModel {
        id: suggestionsModel
        ListElement { suggestedUrl: "facebook.com" }
        ListElement { suggestedUrl: "webkit.org" }
    }

    SuggestionItem {
        id: searchItem
        isSearch: true
        anchors {
            top: urlSuggestions.top
            left: urlSuggestions.left
            right: urlSuggestions.right
        }
        onSearchSelected: urlSuggestions.searchSelected()
    }

    ListView {
        id: suggestionsList
        anchors {
            top: searchItem.bottom
            left: urlSuggestions.left
            right: urlSuggestions.right
            bottom: urlSuggestions.bottom
        }
        clip: true
        model: suggestionsModel
        delegate: suggestedItemPrototype
        interactive: false // If we may have more than 2 displayed results, then remove this.
    }

    Component {
        id: suggestedItemPrototype
        SuggestionItem {
            url: suggestedUrl
            width: suggestionsList.width
            onSuggestionSelected: urlSuggestions.suggestionSelected(url)
        }
    }

}
