
//
// ScriptEditorController converts Model data to format for display in View
//  mostly from float, int to string based on format requested.
//

#include <stdio.h>
#include <QtWidgets>
#include <QJSEngine>
#include <QJSValue>
#include <QJSValueIterator>

#include "ScriptEditorController.hh"
#include "ScriptEditorModel.hh"
#include "SoloFunctionsController.hh"
#include <toolsa/LogStream.hh>

ScriptEditorController::ScriptEditorController(ScriptEditorView *view)
{
  // int rows;
  // int cols;


  _currentView = view;
  _currentModel = new ScriptEditorModel();

  //  functionsModel = new SoloFunctionsModel(_currentModel);

    // connect controller slots to model signals 

    // connect model signals to controller slots 
  /*
    connect(table, &QTableWidget::currentItemChanged,
            this, &ScriptEditorController::updateStatus);
    connect(table, &QTableWidget::currentItemChanged,
            this, &ScriptEditorController::updateColor);
    connect(table, &QTableWidget::currentItemChanged,
            this, &ScriptEditorController::updateLineEdit);
    connect(table, &QTableWidget::itemChanged,
            this, &ScriptEditorController::updateStatus);
    connect(formulaInput, &QLineEdit::returnPressed, this, &ScriptEditorController::returnPressed);
    connect(table, &QTableWidget::itemChanged,
            this, &ScriptEditorController::updateLineEdit);
  */
}


ScriptEditorController::ScriptEditorController(ScriptEditorView *view, ScriptEditorModel *model)
{
  // int rows;
  // int cols;


  _currentView = view;

  _currentModel = model;

  //  functionsModel = new SoloFunctionsModel(_currentModel);
  _soloFunctionsController = new SoloFunctionsController(_currentModel->_vol);
  setupSoloFunctions(_soloFunctionsController);

  // connect view signals to controller slots

  connect(_currentView, SIGNAL(needFieldNames()), this, SLOT(needFieldNames()));
  //connect(_currentView, SIGNAL(needDataForField(string, int, int)), 
  //	  this, SLOT(needDataForField(string, int, int)));
  connect(_currentView, SIGNAL(applyVolumeEdits()), 
	  this, SLOT(getVolumeChanges()));


  connect(_currentView, SIGNAL(runOneTimeOnlyScript(QString)),
	  this, SLOT(runOneTimeOnlyScript(QString)));
  connect(_currentView, SIGNAL(runForEachRayScript(QString)),
	  this, SLOT(runForEachRayScript(QString)));


    // connect controller slots to model signals 

    // connect model signals to controller slots 
  /*
    connect(table, &QTableWidget::currentItemChanged,
            this, &ScriptEditorController::updateStatus);
    connect(table, &QTableWidget::currentItemChanged,
            this, &ScriptEditorController::updateColor);
    connect(table, &QTableWidget::currentItemChanged,
            this, &ScriptEditorController::updateLineEdit);
    connect(table, &QTableWidget::itemChanged,
            this, &ScriptEditorController::updateStatus);
    connect(formulaInput, &QLineEdit::returnPressed, this, &ScriptEditorController::returnPressed);
    connect(table, &QTableWidget::itemChanged,
            this, &ScriptEditorController::updateLineEdit);
  */
}



vector<string>  ScriptEditorController::getFieldNames()
{
  vector<string> names = _currentModel->getFields();
  cout << " In ScriptEditorController::getFieldNames, there are " << names.size() << " field names" << endl;
  return names;
}

/*
vector<float> *ScriptEditorController::getData(string fieldName)
{

  LOG(DEBUG) << "getting values for " << fieldName;

  
  //return _currentModel->getData(fieldName);
  
  //  vector<float> ScriptEditorModel::getData(string fieldName)
  vector<float> *data = _currentModel->getData(fieldName);

  LOG(DEBUG) << " found " << data->size() << " data values ";

  return data;
 
}

void ScriptEditorController::setData(string fieldName, vector<float> *data)
{
  LOG(DEBUG) << "setting values for " << fieldName;
  _currentModel->setData(fieldName, data);
}
*/

void  ScriptEditorController::needFieldNames() {
  fieldNamesProvided(getFieldNames());
}

/*
void  ScriptEditorController::needDataForField(string fieldName, int r, int c) {

  _currentView->fieldDataSent(getData(fieldName), r, c);
}
*/


void ScriptEditorController::getVolumeChanges() {

  LOG(DEBUG) << "enter";
  //  QStringList *fieldNames = _currentView->getVariablesFromScriptEditor();
  //int column = 0;
  //for(vector<string>::iterator s = fields->begin(); s != fields->end(); s++) {
    //vector<float> *data = _currentView->getDataForVariableFromScriptEditor(column, *s);
    //setData(*s, data);
    //column++;
  //}
  //volumeUpdated(fieldNames);
  LOG(DEBUG) << "exit";
}

void ScriptEditorController::volumeUpdated(QStringList newFieldNames) {
  //QStringList newFieldNamesFAKE = {"VEL_xyz"};
  //  QStringList newFieldNamesFAKE = {"DBZ", "WIDTH", "ZDR"};
  emit scriptChangedVolume(newFieldNames); // FAKE); // _currentModel->getVolume());
}


void ScriptEditorController::open(string fileName)
{

  // _currentModel->initData(fileName);

  // signal the view to pull the data
  // for each fieldName ...
  // _currentView->newDataReady();
  //  while (_currentModel->moreData()) {  
  //  vector <float> data = _currentModel->getData(fieldName);
  // update display
  //  _currentView->setupContents(data, fieldName);  
  //}
}

// SoloFunctions object comes in with data model already attached 
void ScriptEditorController::setupSoloFunctions(SoloFunctionsController *soloFunctions)
{
  
  QJSValue myExt = engine.newQObject(soloFunctions); // new SoloFunctions());
  engine.globalObject().setProperty("cat", myExt.property("cat"));
  engine.globalObject().setProperty("sqrt", myExt.property("sqrt"));
  engine.globalObject().setProperty("REMOVE_AIRCRAFT_MOTION", myExt.property("REMOVE_AIRCRAFT_MOTION"));
  engine.globalObject().setProperty("BB_UNFOLDING_FIRST_GOOD_GATE", myExt.property("BB_UNFOLDING_FIRST_GOOD_GATE"));
  engine.globalObject().setProperty("ZERO_MIDDLE_THIRD", myExt.property("ZERO_MIDDLE_THIRD"));
  engine.globalObject().setProperty("ZERO_INSIDE_BOUNDARY", myExt.property("ZERO_INSIDE_BOUNDARY"));
  engine.globalObject().setProperty("add", myExt.property("add"));
  engine.globalObject().setProperty("DESPECKLE", myExt.property("DESPECKLE"));
  engine.globalObject().setProperty("SET_BAD_FLAGS_ABOVE", myExt.property("SET_BAD_FLAGS_ABOVE"));

  // print the context ...
  // printQJSEngineContext();
  
}

void ScriptEditorController::printQJSEngineContext() {
                                                                                                           
  LOG(DEBUG) << "current QJSEngine context ...";

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



 /*
void ScriptEditorController::processFormula(QString formula)
{

  // Grab the context before evaluating the formula  
  // ======
  // TODO: YES! This works.  The new global variables are listed here;
  // just find them and add them to the spreadsheet and to the Model??
  // HERE!!!
  // try iterating over the properties of the globalObject to find new variables

  std::map<QString, QString> currentVariableContext;
  QJSValue theGlobalObject = engine.globalObject();

  QJSValueIterator it(theGlobalObject);
  while (it.hasNext()) {
    it.next();
    qDebug() << it.name() << ": " << it.value().toString();
    currentVariableContext[it.name()] = it.value().toString();
  }
  // ======                                                                                                                                    

  QJSValue result = engine.evaluate(text);
  if (result.isArray()) {
    cerr << " the result is an array\n";
    //vector<int> myvector;                                                                                                                      
    //myvector = engine.fromScriptValue(result);                                                                                                 
  }
  cerr << " the result is " << result.toString().toStdString() << endl;

  // ====== 
  // TODO: YES! This works.  The new global variables are listed here;
  // just find them and add them to the spreadsheet and to the Model?? 
  // HERE!!!
  // try iterating over the properties of the globalObject to find new variables                                                                 
  QJSValue newGlobalObject = engine.globalObject();

  QJSValueIterator it2(newGlobalObject);
  while (it2.hasNext()) {
    it2.next();
    qDebug() << it2.name() << ": " << it2.value().toString();
    if (currentVariableContext.find(it2.name()) == currentVariableContext.end()) {
      // we have a newly defined variable                                                                                                      
      qDebug() << "NEW VARIABLE " << it2.name() <<  ": " << it2.value().toString();
      addVariableToScriptEditor(it2.name(), it2.value());
    }
  }
  // ======    



}
 */

  /*
void ScriptEditorController::returnPressed()
{
    QString text = formulaInput->text();
    int row = table->currentRow();
    int col = table->currentColumn();
    QTableWidgetItem *item = table->item(row, col);
    if (!item)
        table->setItem(row, col, new ScriptEditorControllerItem(text));
    else
        item->setData(Qt::EditRole, text);
    table->viewport()->update();
}
  */

//emit runOneTimeOnlyScript(oneTimeOnlyScript);
//emit runForEachRayScript(forEachRayScript);

void ScriptEditorController::runOneTimeOnlyScript(QString script)
{
  LOG(DEBUG) << "enter"; 

  /*
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

      LOG(DEBUG) << it.name().toStdString() << ": " << theValue.toStdString(); // it.value().toString().tr\
uncate(100);                                                                                               
      currentVariableContext[it.name()] = it.value().toString();
    }
  */
      // ======                                                                                            
    //    try {
      QJSValue result = engine.evaluate(script);
      if (result.isError()) {
        QString message;
        message.append(result.toString());
        message.append(" on line number ");
        message.append(result.property("lineNumber").toString());
        LOG(DEBUG)
          << "Uncaught exception at line"
          << result.property("lineNumber").toInt()
          << ":" << result.toString().toStdString();
        throw message.toStdString();

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
	/*
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
	*/
      }
      /*
    } catch (const std::exception& ex) {
      criticalMessage(ex.what());
    } catch (const std::string& ex) {
      criticalMessage(ex);
    } catch (...) {
      criticalMessage("Error occurred during evaluation");
    }
      */
      LOG(DEBUG) << "exit";
}


void ScriptEditorController::runForEachRayScript(QString script)
{
  LOG(DEBUG) << "enter";

  QStringList newFieldNames; //  = {"VEL_xyz"}; // , "123", "CDE"};

  
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

      LOG(DEBUG) << it.name().toStdString() << ": " << theValue.toStdString(); // it.value().toString().tr\
uncate(100);                                                                                               
      currentVariableContext[it.name()] = it.value().toString();
    }
  
      // ======                                                                                            
    //    try {

    // for each sweep
   _soloFunctionsController->setCurrentSweepToFirst();

   //while (_soloFunctionsController->moreSweeps()) {
    // for each ray
    _soloFunctionsController->setCurrentRayToFirst();

    while (_soloFunctionsController->moreRays()) {
      LOG(DEBUG) << "more rays ...";
      // calculate boundary mask for each ray? 
      // Yes, when the ray index changes a new boundary mask is calculated 
      // in the SoloFunctionsController
      _soloFunctionsController->applyBoundary();

      QJSValue result = engine.evaluate(script);
      if (result.isError()) {
        QString message;
        message.append(result.toString());
        message.append(" on line number ");
        message.append(result.property("lineNumber").toString());
        LOG(DEBUG)
          << "Uncaught exception at line"
          << result.property("lineNumber").toInt()
          << ":" << result.toString().toStdString();
        throw message.toStdString();

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
	
	
      }
      _soloFunctionsController->nextRay();
    }
    //_soloFunctionsController->nextSweep();
    
    //}
      /*
    } catch (const std::exception& ex) {
      criticalMessage(ex.what());
    } catch (const std::string& ex) {
      criticalMessage(ex);
    } catch (...) {
      criticalMessage("Error occurred during evaluation");
    }
      */

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
	    // COOL! at this point, we have the new field name AND the temporary field name in the RadxVol,
	    // so we can do an assignment now.
            string tempName = theValue.toStdString();
	    string userDefinedName = it2.name().toStdString();
	    // only assign the ray data if this is a Solo Function, f(x)
            size_t length = tempName.length();
            if (length > 0) {
            if (tempName[length-1] == '#') {
              tempName.resize(length-1);
	      _assign(tempName, userDefinedName);
	      // add Variable list ToScriptEditor(it2.name(), it2.value());
	      newFieldNames << it2.name();
	    }
	    }
	  }
        }


    volumeUpdated(newFieldNames);
    LOG(DEBUG) << "exit";
}


void ScriptEditorController::_assign(string tempName, string userDefinedName) {

  // rename the field in the RadxVol
  _soloFunctionsController->assign(tempName, userDefinedName);
}


// request filled by Controller in response to needFieldNames signal                                       
void ScriptEditorController::fieldNamesProvided(vector<string> fieldNames) {

  int useless = 0;

  // fill everything that needs the fieldNames ...                                                         

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
      engine.globalObject().setProperty(fieldName, fieldName);                                           

      /*
      // ===== set field to array of numbers; begin =====
      QJSValue fieldArray = engine.newArray(20);
      QString vectorName = fieldName; //  + "_VECTOR";                                                     
      for (int i=0; i<20; i++) {
        fieldArray.setProperty(i, someValue);
      }
      cout << "adding vector form " << vectorName.toStdString() << endl;
      engine.globalObject().setProperty(vectorName, fieldArray);
      cout << "end adding vector form " << vectorName.toStdString() << endl;
      // ===== set field to array of numbers; end ==== 
      */
      //someValue += 1;                                                                                    

      // //} catch (Exception ex) {                                                                        
      // // cerr << "ERROR - problem setting property on field " << *it << endl;                           
      // //}                                                                                               
    }
    //if (LOG_STREAM_IS_ENABLED(LogStream::DEBUG)) { // causes a segmentation fault                        
    // print the context ...                                                                              \
                                                                                                           
      LOG(DEBUG) << "current QJSEngine context ... after fieldNamesProvided";

      printQJSEngineContext();
}

void ScriptEditorController::addVariableToScriptEditor(QString name, QJSValue value) {

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
      //    sprintf(formattedData, format, data[0]);                                                      \
                                                                                                           
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

