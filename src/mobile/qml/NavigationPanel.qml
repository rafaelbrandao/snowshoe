import QtQuick 2.0
import QtWebKit 3.0
// What to do on Qt5 !?
import "UiConstants.js" as UiConstants
import "tabmanager.js" as TabManager

Item {
    id: navigationPanel
    property bool hasOpennedTabs: false
    signal webViewMaximized()
    signal webViewMinimized()
    property alias urlInputFocus: navigationBar.urlInputFocus
    property alias url: navigationBar.url

    NavigationBar {
        id: navigationBar
        state: "hidden"
    }

    PinchArea {
        id: pinchArea
        visible: false
        anchors.bottom: tabBar.top
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        z: 99

        onPinchFinished: {
            if (pinch.scale > 1.0 && TabManager.overviewGridSize > 1)
                TabManager.overviewGridSize--;
            else if (pinch.scale < 1.0 && TabManager.overviewGridSize < TabManager.MAX_GRID_SIZE)
                TabManager.overviewGridSize++;
            else
                return;
            TabManager.doTabOverviewLayout();
        }
    }

    Item {
        id: tabBar
        width: UiConstants.PortraitWidth
        height: 57
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        z: 1

        Image {
            source: ":/mobile/tabs/bg_image"
            anchors.fill: parent
        }

        Item {
            id: tabBarRow
            anchors.centerIn: parent
        }
        MouseArea {
            property int lastX
            property int lastY
            anchors.fill: parent

            onPressed: {
                lastX = mouse.x
                lastY = mouse.y
            }

            onReleased: {
                if (Math.abs(mouse.y - lastY) > height * 3
                    || Math.abs(mouse.x - lastX) < UiConstants.DefaultSwipeLenght) {
                    navigationPanel.webViewMinimized();
                    return;
                }

                if (mouse.x > lastX) // swipe right
                    TabManager.goToPreviousTab();
                else // swipe left
                    TabManager.goToNextTab();

                navigationBar.update()
                navigationPanel.webViewMaximized()
            }
        }

        states: State {
            name: "hidden"
            PropertyChanges { target: tabBar; visible: false; }
        }
    }

    function openUrl(url, shouldOpenNewTab)
    {
        TabManager.NAVBAR_HEIGHT = navigationBar.navBarHeight;
        var tab = shouldOpenNewTab ? TabManager.createTab(url, navigationPanel, tabBarRow) : TabManager.getCurrentTab()
        // FIXME: we cannot set the same url to a webview while it is loading.
        // BUG: https://bugs.webkit.org/show_bug.cgi?id=82506
        if (shouldOpenNewTab) {
            var statusBarIndicator = tab.statusIndicator;
            statusBarIndicator.anchors.verticalCenter = tabBarRow.verticalCenter
            var tabCount = TabManager.tabCount()
            var indicatorSpacing = tabCount * 4
            tabBarRow.width = ((tabCount + 1) * statusBarIndicator.width) + indicatorSpacing
            statusBarIndicator.x = (tabCount * statusBarIndicator.width) + indicatorSpacing
            navigationPanel.hasOpennedTabs = true;
            tab.fullScreenRequested.connect(webViewMaximized);
            tab.closeTabRequested.connect(closeCurrentTab);
        } else {
            tab.url = url;
        }
        navigationPanel.webViewMaximized();
        navigationBar.update(url)
        return tab;
    }

    function setFullScreen(value)
    {
        if (value) {
            TabManager.currentTabLayout = TabManager.FULLSCREEN_LAYOUT;
            tabBar.state = "";
            if (navigationBar.state == "hidden") {
                navigationBar.state = "visible";
            } else {
                navigationBar.state = "hidden";
                return;
            }
        } else {
            TabManager.currentTabLayout = TabManager.OVERVIEW_LAYOUT;
            tabBar.state = "hidden";
            navigationBar.state = "hidden";
        }
        TabManager.setTabLayout(TabManager.currentTabLayout, 1);
    }

    function closeCurrentTab()
    {
        TabManager.removeTab(TabManager.currentTab)
        if (TabManager.tabCount() === 0)
            hasOpennedTabs = false;
    }
}
