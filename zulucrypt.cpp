/*
 * 
 *  Copyright (c) 2011
 *  name : mhogo mchungu 
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "version.h"
#include "zulucrypt.h"
#include "ui_zulucrypt.h"
#include <QProcess>
#include <QStringList>
#include <QMenu>
#include <QCursor>
#include <QByteArray>
#include <QColor>
#include <iostream>
#include <QMessageBox>

#include <stdio.h>

zuluCrypt::zuluCrypt(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::zuluCrypt)
{
	zuluCryptExe = "/usr/bin/zuluCrypt-cli" ;

	openFileUI.setParent(this);
	openFileUI.setWindowFlags(Qt::Window | Qt::Dialog);

	openPartitionUI.setParent(this);
	openPartitionUI.setWindowFlags(Qt::Window | Qt::Dialog);

	item_count = 0 ;

	ui->setupUi(this);
	this->setFixedSize(this->size());

	ui->tableWidget->setColumnWidth(0,246);

	ui->tableWidget->setColumnWidth(1,356);

	m = new QMenu ;

	connect(m->addAction("close"),SIGNAL(triggered()),this,SLOT(close())) ;

	connect(m->addAction("properties"),SIGNAL(triggered()),this,SLOT(volume_property())) ;

	connect(ui->actionFileOpen,SIGNAL(triggered()),this,SLOT(showOpenFileDialogClear())) ;

	connect(ui->actionPartitionOpen,SIGNAL(triggered()),this,SLOT(showOpenPartitionDialog()));

	connect(openFileUI.passphraseDialogUI.PushButtonOpen,SIGNAL(clicked()),this,SLOT(open()));

	connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(options(QTableWidgetItem*))) ;

	connect((QObject *)&openPartitionUI,SIGNAL(clickedPartition(QString)),(QObject *)&openFileUI,SLOT(clickedPartitionOption(QString)));


	connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(aboutMenuOption())) ;

	setUpOpenedVolumes() ;
}

void zuluCrypt::aboutMenuOption(void)
{
	QMessageBox m ;

	m.addButton(QMessageBox::Ok);

	m.setText(QString( VERSION_STRING )) ;

	m.exec() ;
}

void zuluCrypt::showOpenPartitionDialog(void)
{
	openPartitionUI.showPartitionsUI();
}

void zuluCrypt::setUpOpenedVolumes(void)
{

	QStringList Z =  QDir(QString("/dev/mapper")).entryList().filter("zuluCrypt-") ;

	char *c, *d, *v,*volume,*N ;
	QProcess * p ;
	for ( int i = 0 ; i < Z.size() ; i++){

		c = Z.at(i).toAscii().data() ;

		d = c + strlen( c )  ;

		while( *--d != '-') { ; }

		d++ ;

		p = new QProcess() ;

		p->start(zuluCryptExe + " status " + QString(d)) ;

		p->waitForReadyRead() ;

		c = p->readAllStandardOutput().data() ;

		p->close();

		p->deleteLater();

		d = strstr(c,"device:") + 9 ;

		if (strncmp(d ,"/dev/loop",9) != 0 ){
			v = d ;
		}else{
			v = strstr(c,"loop:") + 9 ;
		}

		d = v ;

		while ( *++d != '\n') { ; }

		*d = '\0' ;

		N = d = volume = new char[ strlen( v ) + 1 ] ;

		while ( ( *d++ = *v++ ) != '\0') { ; }

		p = new QProcess() ;

		p->start("mount");

		p->waitForReadyRead() ;

		c = p->readAllStandardOutput().data() ;

		v = strrchr(volume,'/') + 1 ;

		if( ( d = strstr(c,v ) ) == NULL ){
			UIMessage(QString("WARNING: An inconsitency is detected, " + QString(N) + QString(" is opened but not mounted")));
			continue ;
		}

		v = d = d + strlen(v) + 4 ;

		while ( *++v != ' ') { ; }

		*v = '\0' ;

		addItemToTable(QString(volume),QString(d)) ;

		delete [] volume ;

		p->close();

		p->deleteLater();
	}
}

void zuluCrypt::addItemToTable(QString x,QString y)
{
	ui->tableWidget->insertRow(item_count);
	ui->tableWidget->setItem(item_count,0,new QTableWidgetItem(x)) ;
	ui->tableWidget->setItem(item_count,1,new QTableWidgetItem(y)) ;

	ui->tableWidget->item(item_count,0)->setTextAlignment(Qt::AlignCenter);
	ui->tableWidget->item(item_count,1)->setTextAlignment(Qt::AlignCenter);

	item_count++ ;
}

void zuluCrypt::removeRowFromTable( int x )
{
	ui->tableWidget->removeRow( x ) ;
	item_count-- ;
}

void zuluCrypt::volume_property()
{
	QProcess p ;

	QString z = zuluCryptExe + " status " + item->tableWidget()->item(item->row(),0)->text() ;

	p.start( z ) ;
	p.waitForFinished() ;
	QString M = p.readAllStandardOutput() ;

	char *c = M.toAscii().data() ;

	int i = 0 ;

	while ( *c++ != '\n') { i++ ; }

	QMessageBox m ;
	m.setText( M.right( M.size() - i) ) ;
	m.addButton(QMessageBox::Ok);
	m.exec() ;
}

void zuluCrypt::options(QTableWidgetItem* t)
{
	item = t ;
	m->exec(QCursor::pos()) ;
}

void zuluCrypt::UIMessage(QString message)
{
	QMessageBox m ;
	m.setText(message);
	m.addButton(QMessageBox::Ok);
	m.exec() ;
}

void zuluCrypt::close(void)
{
	QProcess p ;

	p.start(zuluCryptExe + " close " + ui->tableWidget->item(item->row(),0)->text() ) ;
	p.waitForFinished() ;

	switch ( p.exitCode() ) {
	case 0 : removeRowFromTable(item->row()) ;
		break ;

	case 1 : UIMessage("ERROR: close failed, encrypted volume with that name does not exist") ;
		break ;

	case 2 : UIMessage("ERROR: close failed, the mount point and/or one or more files are in use");
		break ;
	default :
		;
	}
}

void zuluCrypt::open()
{
	if (openFileUI.volume_path.isEmpty() == true){

		UIMessage(QString("ERROR: The volume path field is empty"));
		openFileUI.passphraseDialogUI.OpenVolumePath->setFocus();
		openFileUI.show();
		return ;
	}

	if (openFileUI.mount_point_path.isEmpty() == true){

		UIMessage(QString("ERROR: The mount point path field is empty"));
		openFileUI.passphraseDialogUI.MountPointPath->setFocus();
		openFileUI.show();
		return ;
	}

	if (openFileUI.passphrase.isEmpty() == true){

		UIMessage(QString("ERROR: The passphrase field is empty"));
		openFileUI.passphraseDialogUI.PassPhraseField->setFocus();
		openFileUI.show();
		return ;
	}

	QString program ;
	if ( openFileUI.boolPassphraseFromFile == true)	{

		program = zuluCryptExe + " open " + openFileUI.volume_path + " " + openFileUI.mount_point_path + " " + openFileUI.mode + " -f " + openFileUI.passphrase ;

	}else{
		program = zuluCryptExe + " open " + openFileUI.volume_path + " " + openFileUI.mount_point_path + " " + openFileUI.mode + " -p " + openFileUI.passphrase ;
	}

	QProcess process ;

	process.start(program) ;
	process.waitForFinished() ;

	int i = process.exitCode() ;

	switch ( i ){
	case 0 : {
		/*
		  There are possible names zuluCrypt-cli will use for mount point and predicting it before hand may
		  cause unnecessary code bloat. If the opening succeed, just go read the output of "mount"
		  and use whatever you will find.
		  */
		char * c = 0 ;

		char *d = 0 ;
		int k ;
		QString N ;
		QProcess Z ;
		Z.start(QString("mount"));

		Z.waitForFinished() ;

		c = Z.readAllStandardOutput().data() ;

		N = "/dev/mapper/zuluCrypt-" + openFileUI.volume_path.split("/").last() ;

		d = N.toAscii().data() ;

		k = strlen( d ) ;

		d = c = strstr( c , d )  + k + 4 ;

		while (*++d != ' ') { ; }

		*d = '\0' ;

		addItemToTable(openFileUI.volume_path,QString( c ));
		} break ;

	case 1 : UIMessage("ERROR: No free loop device to use.") ;
		break ;

	case 2 : UIMessage("ERROR: There seem to be an open volume accociated with given path.");
		break ;

	case 3 : UIMessage("ERROR: No file or device on a given address.") ;
		break ;

	case 4 : UIMessage("ERROR: Wrong passphrase.");
		break ;

	case 5 : UIMessage("ERROR: mount point address is already taken by a file or folder") ;
		break ;
	case 6 : UIMessage("ERROR: passphrase file does not exist");

	case 9 : UIMessage("ERROR: \",\" (comma) is not a valid mount point");
		break ;
	default :
		;
	}
}


void zuluCrypt::showOpenFileDialogClear(void)
{
	openFileUI.clearAllFields();
	openFileUI.showDialog();
}

zuluCrypt::~zuluCrypt()
{
	delete ui;
	delete m ;
}
