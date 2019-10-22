#ifndef DISPLAYFIELDCONTROLLER_H
#define DISPLAYFIELDCONTROLLER_H

#include "DisplayFieldModel.hh"
//#include "ColorMap.hh"

class DisplayFieldController
{

public:

  DisplayFieldController(DisplayFieldModel *model);
  ~DisplayFieldController();

  void addField(DisplayField *newField);
  size_t getNFields();

  DisplayField *getField(size_t fieldIndex);
  DisplayField *getField(string fieldName);


  vector<string> getFieldNames();
  string getSelectedFieldName();
  DisplayField *getSelectedField();
  void setSelectedField(string fieldName);
  void setSelectedField(size_t fieldIndex);


  DisplayField *getFiltered(size_t fieldIndex, int buttonRow);
  size_t getFieldIndex(string fieldName);


  void setForLocationClicked(double value, const string &text);
  void setForLocationClicked(string fieldName, double value, const string &text);
  // DisplayField &getSelectedDisplayField();
  //  bool getChanges();
  ColorMap *getColorMap(string fieldName);
  ColorMap *getColorMap(size_t fieldIndex);
  void setColorMap(string fieldName, ColorMap *newColorMap);
    void colorMapChanged(string newColorMapName);
    bool backgroundChanged(string fieldName);

  void setColorMapMinMax(string fieldName, double min, double max); 
    void colorMapMaxChanged(ColorMap *newColorMap);
    ColorMap *colorMapMaxChanged(double newValue);
    ColorMap *colorMapMinChanged(double newValue);

    string getGridColor();
    void setGridColor(string colorName);
  
    string getEmphasisColor();
    void setEmphasisColor(string colorName);
  
    string getAnnotationColor();
    void setAnnotationColor(string colorName);
  
    string getBackgroundColor();
    void setBackgroundColor(string colorName);

private:
 
  //vector<DisplayField *> *_current;
  DisplayFieldModel *_model; // edit version 
 
  

};

#endif
