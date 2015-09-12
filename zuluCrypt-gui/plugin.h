/*
 *
 *  Copyright ( c ) 2015
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  ( at your option ) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef HMAC_H
#define HMAC_H

#include <QDialog>
#include <QObject>
#include <QString>
#include <QVector>

#include <functional>

class QCloseEvent ;
class QEvent ;

#include "../plugins/plugins.h"

namespace Ui {
class plugin;
}

class plugin : public QDialog
{
	Q_OBJECT
public:
	plugin( QDialog * parent,
	      plugins::type,
	      std::function< void( const QString& ) >,
	      const QString& = QString(),const QVector<QString>& = QVector<QString>() ) ;
	~plugin() ;
	void ShowUI() ;
	void HideUI() ;
private slots:
	void pbSetKey() ;
	void pbSelectKeyFile() ;
	void pbClose() ;
private:
	void enableAll() ;
	void disableAll() ;
	void closeEvent( QCloseEvent * ) ;
	bool eventFilter( QObject * watched,QEvent * event ) ;
	Ui::plugin * m_ui ;
	QString m_key = QString() ;
	std::function< void( const QString& ) > m_function ;
	plugins::type m_pluginType ;
	QVector<QString> m_exe ;
};

#endif // HMAC_H
