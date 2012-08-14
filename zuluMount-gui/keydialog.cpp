#include "keydialog.h"
#include "ui_keydialog.h"

keyDialog::keyDialog( QWidget * parent,QString path,QString mode ) :
	QDialog( parent ),
	m_ui(new Ui::keyDialog)
{
	m_ui->setupUi(this);

	m_path = path ;
	m_mode = mode ;

	m_menu = new QMenu( this ) ;

	this->setFixedSize( this->size() );
	this->setWindowFlags( Qt::Window | Qt::Dialog );
	this->setFont( parent->font() );

	m_ui->lineEditKey->setFocus();

	connect( m_ui->pbCancel,SIGNAL( clicked() ),this,SLOT( pbCancel() ) ) ;
	connect( m_ui->pbOpen,SIGNAL( clicked() ),this,SLOT( pbOpen() ) ) ;
	connect( m_ui->pbOpenKeyFile,SIGNAL( clicked() ),this,SLOT( pbPlugin() ) ) ;
	connect( m_ui->rbKey,SIGNAL( toggled( bool ) ),this,SLOT( rbKey( bool ) ) ) ;
	connect( m_ui->rbKeyFile,SIGNAL( toggled( bool ) ),this,SLOT( rbKeyFile( bool ) ) ) ;
	connect( m_ui->rbPlugIn,SIGNAL( toggled( bool ) ),this,SLOT( rbPlugIn( bool ) ) ) ;
	connect( m_ui->lineEditKey,SIGNAL( textChanged( QString ) ),this,SLOT( keyTextChanged( QString ) ) ) ;
	m_ui->rbKey->setChecked( true ) ;
}

void keyDialog::keyTextChanged( QString txt )
{
	if( m_ui->rbPlugIn->isChecked() ){
		if( txt.contains( QString( "/") ) )
			m_ui->label->setText( tr( "plugin path") );
		else
			m_ui->label->setText( tr( "plugin name") );
	}
}

void keyDialog::enableAll()
{
	m_ui->pbCancel->setEnabled( true );
	m_ui->pbOpen->setEnabled( true );
	m_ui->label->setEnabled( true );
	m_ui->rbKey->setEnabled( true );
	m_ui->rbKeyFile->setEnabled( true );
	m_ui->lineEditKey->setEnabled( true );
	m_ui->rbPlugIn->setEnabled( true );
	if( m_ui->rbPlugIn->isChecked() )
		m_ui->pbOpenKeyFile->setEnabled( true );
}

void keyDialog::disableAll()
{
	m_ui->lineEditKey->setEnabled( false );
	m_ui->pbCancel->setEnabled( false );
	m_ui->pbOpen->setEnabled( false );
	m_ui->pbOpenKeyFile->setEnabled( false );
	m_ui->label->setEnabled( false );
	m_ui->rbKey->setEnabled( false );
	m_ui->rbKeyFile->setEnabled( false );
	m_ui->rbPlugIn->setEnabled( false );
}

void keyDialog::pbPlugin()
{
	QStringList list ;

	// constant is set in "../zuluCrypt-cli/constants.h"
	// current value is "/etc/zuluCrypt/modules"

	QDir dir( QString( ZULUCRYPTpluginPath ) ) ;

	if( !dir.exists() )
		return ;

	list = dir.entryList() ;

	list.removeOne( QString( ".") ) ;
	list.removeOne( QString( "..") ) ;

	m_menu->clear();

	int j = list.size()  ;

	if( j == 0 )
		return ;

	for( int i = 0 ; i < j ; i++ )
		m_menu->addAction( list.at( i ) ) ;

	m_menu->addSeparator() ;

	m_menu->addAction( tr( "cancel" ) ) ;

	connect( m_menu,SIGNAL( triggered( QAction * ) ),this,SLOT( pbPluginEntryClicked( QAction * ) ) ) ;

	m_menu->exec( QCursor::pos() ) ;
}

void keyDialog::pbPluginEntryClicked( QAction * e )
{
	if( e->text() != tr( "cancel" ) )
		m_ui->lineEditKey->setText( e->text() ) ;
}

void keyDialog::closeEvent( QCloseEvent * e )
{
	e->ignore();
	this->HideUI();
}

void keyDialog::slotMountComplete( int st,QString m )
{
	if( st == 0 ){
		emit updatePartitions();
		this->HideUI();
	}else{
		DialogMsg msg( this ) ;

		msg.ShowUIOK( QString( "ERROR" ),m ) ;
		m_ui->lineEditKey->clear();
		m_ui->lineEditKey->setFocus();
		this->enableAll();
	}
}

void keyDialog::pbOpen()
{
	if( m_ui->lineEditKey->text().isEmpty() ){
		DialogMsg msg( this ) ;
		msg.ShowUIOK( tr( "ERROR" ),tr( "passphrase field is empty" ) ) ;
		m_ui->lineEditKey->setFocus();
		return ;
	}

	QString m ;
	if( m_ui->rbKey->isChecked() )
		m = QString( "-p ") + m_ui->lineEditKey->text().replace( "\"","\"\"\"" ) ;
	else if( m_ui->rbKeyFile->isChecked() )
		m = QString( "-f ") + m_ui->lineEditKey->text().replace( "\"","\"\"\"" ) ;
	else if( m_ui->rbPlugIn->isChecked() )
		m = QString( "-G ") + m_ui->lineEditKey->text().replace( "\"","\"\"\"" ) ;

	managepartitionthread * part = new managepartitionthread() ;
	connect( part,SIGNAL( signalMountComplete( int,QString ) ),this,SLOT( slotMountComplete( int,QString ) ) ) ;

	part->setDevice( m_path );
	part->setMode( m_mode );
	part->setKeySource( m );

	part->startAction( QString( "cryptoOpen" ) ) ;
	this->disableAll();
}

void keyDialog::rbPlugIn( bool opt )
{
	if( opt ){
		m_ui->lineEditKey->setEchoMode( QLineEdit::Normal );
		m_ui->label->setText( tr( "plugin name" ) );
		m_ui->pbOpenKeyFile->setIcon( QIcon( QString( ":/module.png" ) ) );

		m_ui->pbOpenKeyFile->setEnabled( true );
		m_ui->lineEditKey->clear();
	}
}

void keyDialog::rbKey( bool opt )
{
	if( opt ){
		m_ui->label->setText( tr( "key" ) );
		m_ui->lineEditKey->setEchoMode( QLineEdit::Password );
		m_ui->pbOpenKeyFile->setIcon( QIcon( QString( ":/passphrase.png" ) ) );
		m_ui->pbOpenKeyFile->setEnabled( false );
		m_ui->lineEditKey->clear();
	}
}

void keyDialog::rbKeyFile( bool opt )
{
	if( opt ){
		m_ui->lineEditKey->setEchoMode( QLineEdit::Normal );
		m_ui->label->setText( tr( "keyfile path" ) );
		m_ui->pbOpenKeyFile->setIcon( QIcon( QString( ":/keyfile.png" ) ) );
		m_ui->pbOpenKeyFile->setEnabled( true );
		m_ui->lineEditKey->clear();
	}
}

void keyDialog::pbCancel()
{
	this->HideUI();
}

void keyDialog::ShowUI()
{
	this->show();
}

void keyDialog::HideUI()
{
	emit hideUISignal();
	this->hide();
}

keyDialog::~keyDialog()
{
	m_menu->deleteLater();
	delete m_ui;
}