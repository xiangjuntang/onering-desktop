#include <QDebug>
#include "oneringview.h"
#include "networkaccessmanager.h"
#include "jsapi.h"

OneRingView::OneRingView(const QUrl &url, int width, int height, QVariantMap &props)
	: QWebView(), contextMenuEnabled(false)
{
	QNetworkAccessManager *oldManager = page()->networkAccessManager();
	NetworkAccessManager *newManager = new NetworkAccessManager(this, oldManager);
	page()->setNetworkAccessManager(newManager);

#ifdef DEBUG
	connect(this, SIGNAL(urlChanged(const QUrl &)),
		this, SLOT(printCurrentUrl(const QUrl &)));
#endif

	connect(this, SIGNAL(titleChanged(const QString &)),
		this, SLOT(setWindowTitle(const QString &)));

	JsApi *jsapi = new JsApi(this);
	jsapi->setWebView(this);
	jsapi->setWindow(this);

	this->setAttribute(Qt::WA_DeleteOnClose, true);

	load(url);
	resize(width, height);

	if (props["fixedSize"].toBool()) {
		setFixedSize(width, height);
	}
	if (props["popup"].toBool()) {
		setWindowFlags(Qt::Popup);
	}
	if (!props["title"].toString().isEmpty()) {
		setWindowTitle(props["title"].toString());
	}
}

void OneRingView::printCurrentUrl(const QUrl &url)
{
	qDebug() << "nav to:" << url.toString();
}

void OneRingView::contextMenuEvent(QContextMenuEvent *ev)
{
	if (contextMenuEnabled) {
		QWebView::contextMenuEvent(ev);
	}
}

void OneRingView::enableContextMenu()
{
	contextMenuEnabled = true;
}

void OneRingView::disableContextMenu()
{
	contextMenuEnabled = false;
}
