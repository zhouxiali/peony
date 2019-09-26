#include "directory-view-container.h"
#include "directory-view-plugin-iface.h"
#include "directory-view-factory-manager.h"
#include "standard-view-proxy.h"
#include "file-utils.h"

#include <QVBoxLayout>

using namespace Peony;

DirectoryViewContainer::DirectoryViewContainer(QWidget *parent) : QWidget(parent)
{
    m_proxy = new DirectoryView::StandardViewProxy;

    setContentsMargins(0, 0, 0, 0);
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

DirectoryViewContainer::~DirectoryViewContainer()
{
    m_proxy->closeProxy();
    if (m_proxy->getView())
        m_proxy->getView()->closeView();
}

bool DirectoryViewContainer::canGoBack()
{
    return !m_back_stack.isEmpty();
}

void DirectoryViewContainer::goBack()
{
    if (!canGoBack())
        return;

    auto uri = m_back_stack.pop();
    m_forward_stack.push(uri);
    goToUri(uri, false);
}

bool DirectoryViewContainer::canGoForward()
{
    return !m_forward_stack.isEmpty();
}

void DirectoryViewContainer::goForward()
{
    if (!canGoForward())
        return;

    auto uri = m_forward_stack.pop();
    m_back_stack.push(uri);
    goToUri(uri, false);
}

bool DirectoryViewContainer::canCdUp()
{
    return !FileUtils::getParentUri(m_proxy->getDirectoryUri()).isNull();
}

void DirectoryViewContainer::cdUp()
{
    auto uri = FileUtils::getParentUri(m_proxy->getDirectoryUri());
    if (uri.isNull())
        return;

    goToUri(uri, true);
}

void DirectoryViewContainer::goToUri(const QString &uri, bool addHistory)
{
    if (addHistory) {
        m_forward_stack.clear();
        m_back_stack.push(uri);
    }
    m_proxy->setDirectoryUri(uri);
    m_proxy->beginLocationChange();
    //m_active_view_prxoy->setDirectoryUri(uri);
}

void DirectoryViewContainer::switchViewType(const QString &viewId)
{
    if (!m_proxy)
        return;

    if (m_proxy->getView()) {
        if (viewId == m_proxy->getView()->viewId())
            return;
    }

    auto viewManager = DirectoryViewFactoryManager::getInstance();
    auto factory = viewManager->getFactory(viewId);
    if (!factory)
        return;

    auto view = factory->create();
    view->setProxy(m_proxy);

    auto oldView = m_proxy->getView();
    if (oldView) {
        m_layout->removeWidget(dynamic_cast<QWidget*>(oldView));
    }
    m_proxy->switchView(view);
    m_layout->addWidget(dynamic_cast<QWidget*>(view), Qt::AlignBottom);
}

void DirectoryViewContainer::refresh()
{
    //m_active_view_prxoy->setDirectoryUri(currentUri)
}

void DirectoryViewContainer::bindNewProxy(DirectoryViewProxyIface *proxy)
{
    //disconnect old proxy
    //connect new proxy
}
