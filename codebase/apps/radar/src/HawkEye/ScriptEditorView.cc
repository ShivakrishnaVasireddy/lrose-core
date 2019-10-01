
#include <stdio.h>
#include <QtWidgets>
#include <QMessageBox>
#include <QModelIndex>
#include <QJSEngine>
#include <QJSValue>
#include <QJSValueIterator>
#include <vector>
#include <iostream>
#include <toolsa/LogStream.hh>
//#include "TextEdit.hh"
#include "ScriptEditorView.hh"
//#include "ScriptEditorDelegate.hh"
//#include "ScriptEditorItem.hh"
#include "SoloFunctions.hh"
#include "DataField.hh"


using namespace std;

Q_DECLARE_METATYPE(QVector<int>)
Q_DECLARE_METATYPE(QVector<double>)


// ScriptEditorView will emit signals that are followed by the controller
//
//


  ScriptEditorView::ScriptEditorView(QWidget *parent)
  : QMainWindow(parent)
{
  LOG(DEBUG) << "in ScriptEditorView constructor";
  //  initScriptEditor();
  int rows;
  int cols;

  cols = 3; // (int) fieldNames.size();
  rows = 200;

    addToolBar(toolBar = new QToolBar());
    formulaInput = new TextEdit(this);
    // get the font to determine height of one row
    QFontMetrics m(formulaInput->font());
    int rowHeight = m.lineSpacing();
    formulaInput->setFixedHeight(3*rowHeight);
    cellLabel = new QLabel(toolBar);
    //cellLabel->setMaximumSize(50, 10);
    //cellLabel->setMinimumSize(80, 10);

    toolBar->addWidget(cellLabel);
    toolBar->addWidget(formulaInput);

    int actionFontSize = 14;

    QAction *cancelAct = new QAction(tr("&Cancel"), this);
    cancelAct->setStatusTip(tr("Cancel changes"));
    cancelAct->setIcon(QIcon(":/images/cancel_x.png"));
    connect(cancelAct, &QAction::triggered, this, &ScriptEditorView::cancelFormulaInput);
    toolBar->addAction(cancelAct);

    QAction *okAct = new QAction(tr("&Ok"), this);
    okAct->setStatusTip(tr("Accept changes"));
    okAct->setIcon(QIcon(":/images/ok_check.png"));
    connect(okAct, &QAction::triggered, this, &ScriptEditorView::acceptFormulaInput);
    toolBar->addAction(okAct);

    QAction *applyAct = new QAction(tr("&Apply"), this);
    applyAct->setStatusTip(tr("Apply changes to display"));
    applyAct->setIcon(QIcon(":/images/apply.png"));
    QFont applyFont = applyAct->font();
    applyFont.setBold(true);
    applyFont.setPointSize(actionFontSize);
    applyAct->setFont(applyFont);
    connect(applyAct, &QAction::triggered, this, &ScriptEditorView::applyChanges);
    toolBar->addAction(applyAct);

    createActions();
    LOG(DEBUG) << "Action created\n";
    //updateColor(0);
    //LOG(DEBUG) << "update Color\n";
    setupMenuBar();
    LOG(DEBUG) << "setupMenuBar\n";
    //setupContentsBlank();
    //cout << "setupContentsBlank\n";
    setupContextMenu();
    cout << "setupContextMenu\n";
    //setCentralWidget(table);
    cout << "setCentralWidgets\n";

    statusBar();
    //connect(table, &QTableWidget::currentItemChanged,
    //        this, &ScriptEditorView::updateStatus);
    //connect(table, &QTableWidget::currentItemChanged,
    //        this, &ScriptEditorView::updateColor);
    //connect(table, &QTableWidget::currentItemChanged,
    //        this, &ScriptEditorView::updateTextEdit);
    //connect(table, &QTableWidget::itemChanged,
    //        this, &ScriptEditorView::updateStatus);
    //    connect(formulaInput, &QTextEdit::returnPressed, this, &ScriptEditorView::returnPressed);
    // connect(formulaInput, &TextEdit::Pressed, this, &ScriptEditorView::returnPressed);
    //connect(table, &QTableWidget::itemChanged,
    //        this, &ScriptEditorView::updateTextEdit);


    QString title("Script Editor");
    //title.append(QString::number(rayAzimuth, 'f', 2));
    //title.append(" degrees");
    setWindowTitle(title);

    //setupSoloFunctions();
}




void ScriptEditorView::init()
{
  LOG(DEBUG) << "emitting signal to get field names";
  //  emit a signal to the controller to get the data for display
  emit needFieldNames();
  
  //setupSoloFunctions();
}

void ScriptEditorView::createActions()
{
}

void ScriptEditorView::setupMenuBar()
{
}

void ScriptEditorView::updateStatus(QTableWidgetItem *item)
{
  //    if (item && item == table->currentItem()) {
  //      statusBar()->showMessage(item->data(Qt::StatusTipRole).toString(), 1000);
        //cellLabel->setText(tr("Cell: (%1)").arg(ScriptEditorUtils::encode_pos(table->row(item), table->column(item))));
  //  }
}


void ScriptEditorView::updateTextEdit(QTableWidgetItem *item)
{
  //    if (item != table->currentItem())
  //      return;
    if (item)
        formulaInput->setText(item->data(Qt::EditRole).toString());
    else
        formulaInput->clear();
}

void ScriptEditorView::returnPressed()
{
    QString text = formulaInput->getText();
    LOG(DEBUG) << "text entered: " << text.toStdString();

    //    int row = table->currentRow();
    //int col = table->currentColumn();
    //QTableWidgetItem *item = table->item(row, col);
    //if (!item)
    //    table->setItem(row, col, new ScriptEditorItem(text));
    //else
    //    item->setData(Qt::EditRole, text);
    //table->viewport()->update();
}

float  ScriptEditorView::myPow()
{
  return(999.9);
}

// SoloFunctions object comes in with data model already attached
void ScriptEditorView::setupSoloFunctions(SoloFunctions *soloFunctions) {

  LOG(DEBUG) << "defining REMOVE_AIRCRAFT_MOTION function";
  //  emit radarVolumeDataRequest();  make the request for the data inside the SoloFunctions object

  //    QJSValue myExt = engine.newQObject(new SoloFunctions(_controller));
  QJSValue myExt = engine.newQObject(soloFunctions); // new SoloFunctions());
    engine.globalObject().setProperty("cat", myExt.property("cat"));
    engine.globalObject().setProperty("sqrt", myExt.property("sqrt"));
    engine.globalObject().setProperty("REMOVE_AIRCRAFT_MOTION", myExt.property("REMOVE_AIRCRAFT_MOTION"));
    engine.globalObject().setProperty("add", myExt.property("add"));

    // print the context ...
    printQJSEngineContext();

    /*
    LOG(DEBUG) << "current QJSEngine context ...";

    std::map<QString, QString> currentVariableContext;
    QJSValue theGlobalObject = engine.globalObject();

      QJSValueIterator it(theGlobalObject);
      while (it.hasNext()) {
	it.next();
        QString theValue = it.value().toString();
        theValue.truncate(100);

	qDebug() << it.name() << ": " << theValue; // it.value().toString().truncate(100);
        currentVariableContext[it.name()] = it.value().toString();
      }
      LOG(DEBUG) << "end current QJSEngine context";
    */
  
}

void ScriptEditorView::applyChanges()
{
  // TODO: send a list of the variables in the GlobalObject of the
  // QJSEngine to the model (via the controller?)
  emit applyVolumeEdits();
}

void ScriptEditorView::acceptFormulaInput()
{
    QString text = formulaInput->getText();
    cerr << "text entered: " << text.toStdString() << endl;
    
    // Grab the context before evaluating the formula
    //  YES! This works.  The new global variables are listed here;
    // just find them and add them to the spreadsheet and to the Model??
    // HERE!!!
    // try iterating over the properties of the globalObject to find new variables
    std::map<QString, QString> currentVariableContext;
    QJSValue theGlobalObject = engine.globalObject();

    QJSValueIterator it(theGlobalObject);
    while (it.hasNext()) {
      it.next();
      QString theValue = it.value().toString();
      theValue.truncate(100);

      LOG(DEBUG) << it.name().toStdString() << ": " << theValue.toStdString(); // it.value().toString().truncate(100);
      currentVariableContext[it.name()] = it.value().toString();
    }
      // ======
    try {
      QJSValue result = engine.evaluate(text);
      if (result.isError()) {
        QString message;
        message.append(result.toString());
        message.append(" on line number ");
        message.append(result.property("lineNumber").toString());
        criticalMessage(message.toStdString()); 
        LOG(DEBUG)
	  << "Uncaught exception at line"
  	  << result.property("lineNumber").toInt()
	  << ":" << result.toString().toStdString();
      } else {

	LOG(DEBUG) << " the result is " << result.toString().toStdString();

	if (result.isArray()) {
	  cerr << " the result is an array\n"; 
        //vector<int> myvector;
        //myvector = engine.fromScriptValue(result);
	} 
        if (result.isNumber()) {
	  cerr << " the result is a number " << result.toString().toStdString() << endl;
          //setSelectionToValue(result.toString());
        }

      // ======
      //  YES! This works.  The new global variables are listed here;
      // just find them and add them to the spreadsheet and to the Model??
      // HERE!!!
      // try iterating over the properties of the globalObject to find new variables
	QJSValue newGlobalObject = engine.globalObject();

	QJSValueIterator it2(newGlobalObject);
	while (it2.hasNext()) {
	  it2.next();
          QString theValue = it2.value().toString();
          theValue.truncate(100);
	  LOG(DEBUG) << it2.name().toStdString() << ": " << theValue.toStdString();
	  if (currentVariableContext.find(it2.name()) == currentVariableContext.end()) {
	    // we have a newly defined variable
	    LOG(DEBUG) << "NEW VARIABLE " << it2.name().toStdString() <<  ": " << theValue.toStdString();
	    addVariableToScriptEditor(it2.name(), it2.value());
	  }
	}
	// ======
        /*
	int row = table->currentRow();
	int col = table->currentColumn();
	QTableWidgetItem *item = table->item(row, col);
	if (!item) {
          LOG(DEBUG) << "considered not item ";
	  table->setItem(row, col, new ScriptEditorItem(result.toString())); // text));
        } else {
          LOG(DEBUG) << "considered item";
	  item->setData(Qt::EditRole, result.toString()); // text);
        }
        */
	//table->viewport()->update();
      }
    } catch (const std::exception& ex) {
      criticalMessage(ex.what());
    } catch (const std::string& ex) {
      criticalMessage(ex);
    } catch (...) {
      criticalMessage("Error occurred during evaluation");
    }

}

void ScriptEditorView::cancelFormulaInput()
{
  
  //QString text = formulaInput->getText();
  cerr << "cancelling formula changes" << endl;
    /*
    int row = table->currentRow();
    int col = table->currentColumn();
    QTableWidgetItem *item = table->item(row, col);
    if (!item)
        table->setItem(row, col, new ScriptEditorItem(text));
    else
        item->setData(Qt::EditRole, text);
    table->viewport()->update();
    */
}


// TODO: I have to be careful here ...
// addField will always work, it just renames the field if it already
// exists.  If the field hasn't changed, then don't send it in the list.
// Hmmm, how to detect and keep track of changes?  
// 1) Just always add new field and then allow a delete of a field? 
//     Uck, this requires another dialog.
// 2) Select which fields to write to save file?
//     Uck, same ... requires another dialog
// 3) Save them all; use RadxConvert to select fields.
//     Easy, simple. Do it!
//
// Hmm, maybe we want to get the information from the spreadsheet table?
// Yes, get them from spreadsheet (QTableWidget) NOT from the QJSEngine.
// QJSEngine has a bunch of context variables and functions, which we don't want.
vector<string> *ScriptEditorView::getVariablesFromScriptEditor() {

  vector<string> *names = new vector<string>;

  
  // try iterating over the properties of the globalObject to find new variables
  //QJSValue newGlobalObject = engine.globalObject();

  //QJSValueIterator it2(newGlobalObject);
  //while (it2.hasNext()) {
  //  it2.next();
    //QString theValue = it2.value().toString(); // TODO: this could be the bottle neck; try sending list of double?
    //theValue.truncate(100);

  /* TODO: we'll need to get the list of field variables from somewhere ... 
  for (int c=0; c < table->columnCount(); c++) {
    QTableWidgetItem *tableWidgetItem = table->horizontalHeaderItem(c);
    string fieldName = tableWidgetItem->text().toStdString(); 
    LOG(DEBUG) << fieldName; 
    //if (currentVariableContext.find(it2.name()) == currentVariableContext.end()) {
    //  // we have a newly defined variable
    //  qDebug() << "NEW VARIABLE " << it2.name() <<  ": " << theValue; // it2.value().toString().truncate(100);
    //  addVariableToScriptEditor(it2.name(), it2.value());
    //}
    names->push_back(fieldName);
  }
  */
  names->push_back("TODO: getFieldNames");
  return names;
}

//
// Get data from spreadsheet/table because we need to capture individual cell edits
//
/*
vector<float> *ScriptEditorView::getDataForVariableFromScriptEditor(int column, string fieldName) {

  vector<float> *data = new vector<float>;

  int c = 0;
  // QTableWidgetItem *tableWidgetItem = table->horizontalHeaderItem(c);
  // TODO; verify fieldName and matches expected name
  LOG(DEBUG) << "getting data for column " << column << ", " << fieldName;;
  // go through the rows and put the data into a vector
  for (int r = 0; r < table->rowCount(); r++) {
    QTableWidgetItem *tableWidgetItem = table->item(r, c);
    bool ok;
    float value = tableWidgetItem->text().toFloat(&ok);
    if (ok) {
      data->push_back(value);
      LOG(DEBUG) << value;
    } else {
      QMessageBox::warning(this, tr("HawkEye"),
                           tr("Could not convert to number.\n"),
                           QMessageBox::Abort);
    }
    
  }
	
  return data;
}
*/

/*
void ScriptEditorView::setSelectionToValue(QString value)
{
  //    QTableWidgetItem *item = table->currentItem();
  //    QColor col = item ? item->backgroundColor() : table->palette().base().color();
  //    col = QColorDialog::getColor(col, this);
  //    if (!col.isValid())
  //        return;

    QList<QTableWidgetItem*> selected = table->selectedItems();
    if (selected.count() == 0)
        return;

    foreach (QTableWidgetItem *i, selected) {
        if (i)
          i->setText(value); // setBackgroundColor(col);
    }

    //updateColor(table->currentItem());
}
*/

/*
void ScriptEditorView::selectColor()
{
    QTableWidgetItem *item = table->currentItem();
    QColor col = item ? item->backgroundColor() : table->palette().base().color();
    col = QColorDialog::getColor(col, this);
    if (!col.isValid())
        return;

    QList<QTableWidgetItem*> selected = table->selectedItems();
    if (selected.count() == 0)
        return;

    foreach (QTableWidgetItem *i, selected) {
        if (i)
            i->setBackgroundColor(col);
    }

    updateColor(table->currentItem());
}
*/

/*
bool ScriptEditorView::runFunctionDialog(const QString &title,
                                 const QString &c1Text,
                                 const QString &c2Text,
                                 const QString &opText,
                                 const QString &outText,
                                 QString *cell1, QString *cell2, QString *outCell)
{
    QStringList rows, cols;
    for (int c = 0; c < table->columnCount(); ++c)
        cols << QChar('A' + c);
    for (int r = 0; r < table->rowCount(); ++r)
        rows << QString::number(1 + r);

    QDialog functionDialog(this);
    //functionDialog.setWindowTitle(title);

    TextEdit textEditArea();

    QPushButton cancelButton(tr("Cancel"), &functionDialog);
    connect(&cancelButton, &QAbstractButton::clicked, &functionDialog, &QDialog::reject);

    QPushButton okButton(tr("OK"), &functionDialog);
    okButton.setDefault(true);
    connect(&okButton, &QAbstractButton::clicked, &functionDialog, &QDialog::accept);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(&okButton);
    buttonsLayout->addSpacing(10);
    buttonsLayout->addWidget(&cancelButton);

    QHBoxLayout *dialogLayout = new QHBoxLayout(&functionDialog);
    dialogLayout->addWidget(&textEditArea);
    dialogLayout->addStretch(1);
    dialogLayout->addItem(buttonsLayout);

    if (addDialog.exec()) {
        QString formula = textEditArea.getText();
        return true;
    }

    return false;
}
*/
 /*
bool ScriptEditorView::runInputDialog(const QString &title,
                                 const QString &c1Text,
                                 const QString &c2Text,
                                 const QString &opText,
                                 const QString &outText,
                                 QString *cell1, QString *cell2, QString *outCell)
{
    QStringList rows, cols;
    for (int c = 0; c < table->columnCount(); ++c)
        cols << QChar('A' + c);
    for (int r = 0; r < table->rowCount(); ++r)
        rows << QString::number(1 + r);

    QDialog addDialog(this);
    addDialog.setWindowTitle(title);

    QGroupBox group(title, &addDialog);
    group.setMinimumSize(250, 100);

    QLabel cell1Label(c1Text, &group);
    QComboBox cell1RowInput(&group);
    int c1Row, c1Col;
    ScriptEditorUtils::decode_pos(*cell1, &c1Row, &c1Col);
    cell1RowInput.addItems(rows);
    cell1RowInput.setCurrentIndex(c1Row);

    QComboBox cell1ColInput(&group);
    cell1ColInput.addItems(cols);
    cell1ColInput.setCurrentIndex(c1Col);

    QLabel operatorLabel(opText, &group);
    operatorLabel.setAlignment(Qt::AlignHCenter);

    QLabel cell2Label(c2Text, &group);
    QComboBox cell2RowInput(&group);
    int c2Row, c2Col;
    ScriptEditorUtils::decode_pos(*cell2, &c2Row, &c2Col);
    cell2RowInput.addItems(rows);
    cell2RowInput.setCurrentIndex(c2Row);
    QComboBox cell2ColInput(&group);
    cell2ColInput.addItems(cols);
    cell2ColInput.setCurrentIndex(c2Col);

    QLabel equalsLabel("=", &group);
    equalsLabel.setAlignment(Qt::AlignHCenter);

    QLabel outLabel(outText, &group);
    QComboBox outRowInput(&group);
    int outRow, outCol;
    ScriptEditorUtils::decode_pos(*outCell, &outRow, &outCol);
    outRowInput.addItems(rows);
    outRowInput.setCurrentIndex(outRow);
    QComboBox outColInput(&group);
    outColInput.addItems(cols);
    outColInput.setCurrentIndex(outCol);

    QPushButton cancelButton(tr("Cancel"), &addDialog);
    connect(&cancelButton, &QAbstractButton::clicked, &addDialog, &QDialog::reject);

    QPushButton okButton(tr("OK"), &addDialog);
    okButton.setDefault(true);
    connect(&okButton, &QAbstractButton::clicked, &addDialog, &QDialog::accept);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(&okButton);
    buttonsLayout->addSpacing(10);
    buttonsLayout->addWidget(&cancelButton);

    QVBoxLayout *dialogLayout = new QVBoxLayout(&addDialog);
    dialogLayout->addWidget(&group);
    dialogLayout->addStretch(1);
    dialogLayout->addItem(buttonsLayout);

    QHBoxLayout *cell1Layout = new QHBoxLayout;
    cell1Layout->addWidget(&cell1Label);
    cell1Layout->addSpacing(10);
    cell1Layout->addWidget(&cell1ColInput);
    cell1Layout->addSpacing(10);
    cell1Layout->addWidget(&cell1RowInput);

    QHBoxLayout *cell2Layout = new QHBoxLayout;
    cell2Layout->addWidget(&cell2Label);
    cell2Layout->addSpacing(10);
    cell2Layout->addWidget(&cell2ColInput);
    cell2Layout->addSpacing(10);
    cell2Layout->addWidget(&cell2RowInput);

    QHBoxLayout *outLayout = new QHBoxLayout;
    outLayout->addWidget(&outLabel);
    outLayout->addSpacing(10);
    outLayout->addWidget(&outColInput);
    outLayout->addSpacing(10);
    outLayout->addWidget(&outRowInput);

    QVBoxLayout *vLayout = new QVBoxLayout(&group);
    vLayout->addItem(cell1Layout);
    vLayout->addWidget(&operatorLabel);
    vLayout->addItem(cell2Layout);
    vLayout->addWidget(&equalsLabel);
    vLayout->addStretch(1);
    vLayout->addItem(outLayout);

    if (addDialog.exec()) {
        *cell1 = cell1ColInput.currentText() + cell1RowInput.currentText();
        *cell2 = cell2ColInput.currentText() + cell2RowInput.currentText();
        *outCell = outColInput.currentText() + outRowInput.currentText();
        return true;
    }

    return false;
}
*/

void ScriptEditorView::notImplementedMessage() {
      QMessageBox::information(this, "Not Implemented", "Not Implemented");
}

/*
void ScriptEditorView::actionDisplayCellValues()
{
  notImplementedMessage();
}
void ScriptEditorView::actionDisplayRayInfo()
{
  notImplementedMessage();
}
void ScriptEditorView::actionDisplayMetadata()
{
  notImplementedMessage();
}
void ScriptEditorView::actionDisplayEditHist()
{
  notImplementedMessage();
}
*/

/*
void ScriptEditorView::clear()
{
    foreach (QTableWidgetItem *i, table->selectedItems())
        i->setText("");
}
*/

void ScriptEditorView::setupContextMenu()
{
  //    setContextMenuPolicy(Qt::ActionsContextMenu);
}

/*
void ScriptEditorView::setupContents()
{
    QColor titleBackground(Qt::lightGray);
    QFont titleFont = table->font();
    titleFont.setBold(true);

}
*/

 /*
// request filled by Controller in response to needFieldData 
void ScriptEditorView::fieldDataSent(vector<float> *data, int useless, int c) {
  size_t nPoints = data->size();
  LOG(DEBUG) << "number of data values = " << nPoints;

      string format = "%g";
      char formattedData[250];

      // TODO: get the globalObject for this field and set the values

      //------
      QTableWidgetItem *headerItem = table->horizontalHeaderItem(c);
      QString fieldName = headerItem->text();
      QJSValue fieldArray = engine.newArray(nPoints);
      QString vectorName = fieldName;

      table->setRowCount(nPoints);
      vector<float> dataVector = *data;
      float *dp = &dataVector[0];
      for (int r=0; r<nPoints; r++) {
      // 752019 for (std::size_t r=0; r<data.size(); r++) {
        //    sprintf(formattedData, format, data[0]);
        sprintf(formattedData, "%g", *dp); // data->at(r));
        LOG(DEBUG) << "setting " << r << "," << c << "= " << formattedData; 
        table->setItem(r, c, new ScriptEditorItem(formattedData));
        fieldArray.setProperty(r, *dp); // data.at(r));
        dp++;
      }
      LOG(DEBUG) << "adding vector form " << vectorName.toStdString();
      engine.globalObject().setProperty(vectorName, fieldArray);
      LOG(DEBUG) << "end adding vector form " << vectorName.toStdString();

}
 */

// request filled by Controller in response to needFieldNames signal
void ScriptEditorView::fieldNamesProvided(vector<string> fieldNames) {

  int useless = 0;

  // fill everything that needs the fieldNames ...
  /*
    table->setColumnCount(fieldNames.size());
    LOG(DEBUG) << "In ScriptEditorView::fieldNamesProvided, there are " << fieldNames.size() << " field namess";

    int c = 0;
    for(it = fieldNames.begin(); it != fieldNames.end(); it++) {
      QString the_name(QString::fromStdString(*it));
      LOG(DEBUG) << *it;
      table->setHorizontalHeaderItem(c, new QTableWidgetItem(the_name));
      // TODO: what about setHorizontalHeaderLabels(const QStringList &labels) instead? would it be faster?
      emit needDataForField(*it, useless, c);
      c += 1;
    }

    // test: adding some missing code
    // TODO: magic number 20 = number of rows
    //table->setItemPrototype(table->item(20 - 1, c - 1));
    table->setItemPrototype(table->item(20 - 1, c - 1));
    table->setItemDelegate(new ScriptEditorDelegate());
    // end test: adding some missing code
    */

    // This section of code makes every data field in volume a variable
    // When the variable name is referenced in the formula bar,
    // the variable name as a string is substituted.
    //     
    // for each field in model (RadxVol)

    int someValue = 0;
    vector<string>::iterator it; 

    for(it = fieldNames.begin(); it != fieldNames.end(); it++) {
      QString fieldName(QString::fromStdString(*it));
      // //    try {
      ////QJSValue objectValue = engine.newQObject(new DataField(*it));
      ////engine.globalObject().setProperty(fieldName, objectValue.property("name"));
      //engine.globalObject().setProperty(fieldName, fieldName);

      QJSValue fieldArray = engine.newArray(20);
      QString vectorName = fieldName; //  + "_VECTOR";
      for (int i=0; i<20; i++) {
        fieldArray.setProperty(i, someValue);
      }
      cout << "adding vector form " << vectorName.toStdString() << endl;
      engine.globalObject().setProperty(vectorName, fieldArray);
      cout << "end adding vector form " << vectorName.toStdString() << endl;

      //someValue += 1;

      // //} catch (Exception ex) {
      // // cerr << "ERROR - problem setting property on field " << *it << endl;
      // //}
    }
    
    
    //if (LOG_STREAM_IS_ENABLED(LogStream::DEBUG)) { // causes a segmentation fault
    // print the context ...                                                                                                   
      LOG(DEBUG) << "current QJSEngine context ... after fieldNamesProvided";

      printQJSEngineContext();
}

void ScriptEditorView::addVariableToScriptEditor(QString name, QJSValue value) {

  LOG(DEBUG) << "adding variable to spreadsheet " << name.toStdString();

  string format = "%g";
  // char formattedData[250];

  int variableLength = value.property("length").toInt();
  if ( variableLength > 1) {
    // this is a vector
    LOG(DEBUG) << "variable is a vector " << name.toStdString();
      QJSValue fieldArray = engine.newArray(variableLength);
      QString vectorName = name;
      for (int i=0; i<variableLength; i++) {
        fieldArray.setProperty(i, value.property(i).toInt());
      }
      cout << "adding vector form " << vectorName.toStdString() << endl;
      engine.globalObject().setProperty(vectorName, fieldArray);
      cout << "end adding vector form " << vectorName.toStdString() << endl;
  }

  if (value.isArray()) {
    //qDebug() << "variable isArray " << name << endl;
    LOG(DEBUG) << "variable isArray " << name.toStdString();

    /*
  for(it = value.begin(); it != value.end(); it++) {
    QString the_name(QString::fromStdString(*it));
    cerr << *it << endl;
    table->setHorizontalHeaderItem(c, new QTableWidgetItem(the_name));
    vector<double> data = _controller->getData(*it);
    cerr << "number of data values = " << data.size() << endl;
    for (int r=0; r<20; r++) {
      //    sprintf(formattedData, format, data[0]);                                                                                             
      sprintf(formattedData, "%g", data.at(r));
      cerr << "setting " << r << "," << c << "= " << formattedData << endl;
      table->setItem(r, c, new ScriptEditorItem(formattedData));
    }
    c += 1;
    } */
  }
  if (value.isBool()) {
    //qDebug() << "variable isBool " << name << endl;
    LOG(DEBUG) << "variable isBool " << name.toStdString();
  }
  if (value.isCallable()) {
    //qDebug() << "variable isCallable " << name << endl;
    LOG(DEBUG) << "variable isCallable " << name.toStdString();
  }
  if (value.isDate()) {
    LOG(DEBUG) << "variable isDate " << name.toStdString();
    //qDebug() << "variable isDate " << name << endl;
  }
  if (value.isError()) {
    LOG(DEBUG) << "variable isError " << name.toStdString();
    //qDebug() << "variable isError " << name << endl;
  }
  if (value.isNull()) {
    LOG(DEBUG) << "variable isNull " << name.toStdString();
    //qDebug() << "variable isNull " << name << endl;
  }
  if (value.isNumber()) {
    //qDebug() << "variable isNumber " << name << endl;
    LOG(DEBUG) << "variable isNumber " << name.toStdString();
  }
  if (value.isObject()) {
    LOG(DEBUG) << "variable isObject " << name.toStdString();
    //qDebug() << "variable isObject " << name << endl;
    //    QVector<double> myv = value.property("values");
    //qDebug() << myv.at(0) << ";" << myv.at(1) << endl;
    /*    table->setColumnCount(table->columnCount() + 1);

    int c = table->columnCount() - 1;
    table->setHorizontalHeaderItem(c, new QTableWidgetItem(name));

    QJSValueIterator it(value);
    while (it.hasNext()) {
      it.next();
      LOG(DEBUG) << it.name().toStdString() << ": " << it.value().toString().toStdString();
    }

    for (int r=0; r<value.property("length").toInt(); r++) {
      //qDebug() << it.name() << ": " << it.value().toString();
      QString valueAsString = value.property(r).toString();
      //      sprintf(formattedData, "%g", value.property(r).toInt());
      //table->setItem(r, c, new ScriptEditorItem(formattedData));
      table->setItem(r,c, new QTableWidgetItem(valueAsString));
    }
    */

  }
  // if (value.isQMetaObject()) {
  //   LOG(DEBUG) << "variable isQMetaObject " << name.toStdString();
  //   qDebug() << "variable isQMetaObject " << name << endl;
  // }
  if (value.isQObject()) {
    LOG(DEBUG) << "variable isQObject " << name.toStdString();
    //qDebug() << "variable isQObject " << name << endl;
  }
  if (value.isRegExp()) {
    //qDebug() << "variable isRegExp " << name << endl;
    LOG(DEBUG) << "variable isRegExp " << name.toStdString();
  }
  if (value.isString()) {
    //qDebug() << "variable isString " << name << endl;
    LOG(DEBUG) << "variable isString " << name.toStdString();
    /*
    table->setColumnCount(table->columnCount() + 1);

    int c = table->columnCount() - 1;
    table->setHorizontalHeaderItem(c, new QTableWidgetItem(name));
    table->setItem(0,c, new QTableWidgetItem(value.toString()));
    */
  }
  if (value.isUndefined()) {
    //qDebug() << "variable isUndefined " << name << endl;
    LOG(DEBUG) << "variable isUndefined " << name.toStdString();
  }
  if (value.isVariant()) {
    //qDebug() << "variable isVariant " << name << endl;
    LOG(DEBUG) << "variable isVariant " << name.toStdString();
  }

}

void ScriptEditorView::criticalMessage(std::string message)
{
  QMessageBox::StandardButton reply =
    QMessageBox::critical(this, "QMessageBox::critical()",
                          QString::fromStdString(message),
                          QMessageBox::Ok);
  
  //  QMessageBox::Abort | QMessageBox::Retry | QMessageBox::Ignore);
  if (reply == QMessageBox::Abort)
    LOG(DEBUG) << "Abort";
    // criticalLabel->setText("Abort");
  else if (reply == QMessageBox::Retry)
    LOG(DEBUG) << "Retry";
    // criticalLabel->setText("Retry");
  else
    LOG(DEBUG) << "Ignore";
    // criticalLabel->setText("Ignore");
}



void ScriptEditorView::printQJSEngineContext() {

    // print the context ...                                                                                                   
    LOG(DEBUG) << "current QJSEngine context ...";

    // LOG(DEBUG) << "pepsi cola";
    
    std::map<QString, QString> currentVariableContext;
    QJSValue theGlobalObject = engine.globalObject();

    QJSValueIterator it2(theGlobalObject);
    while (it2.hasNext()) {
      it2.next();
      QString theValue = it2.value().toString();
      theValue.truncate(100);

      LOG(DEBUG) << it2.name().toStdString() << ": " << theValue.toStdString();
      currentVariableContext[it2.name()] = it2.value().toString();
    }
    
    LOG(DEBUG) << "end current QJSEngine context";

}
