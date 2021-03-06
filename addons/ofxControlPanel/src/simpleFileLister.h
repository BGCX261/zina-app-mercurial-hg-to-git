#ifndef SIMPLE_FILE_LISTER
#define SIMPLE_FILE_LISTER

#include "ofMain.h"
#include "ofxDirList.h"

typedef struct{
    string filename;
    string fullpath;
}entry;


class simpleFileLister : public ofxDirList {

     public:

        simpleFileLister();
        int refreshDir();

        int listDir(string directory);

        void reverseOrder();

        bool selectedHasChanged();
        void clearChangedFlag();

		int getNumEntries() { return entries.size(); }
        string getName(int which);
        string getPath(int which);
        void setSelectedFile(int which);
        string getSelectedName();
        string getSelectedPath();

        int  selected;
        bool selectedChanged;
        bool bRevSort;

        string lastDirectory;

        vector <entry> entries;
};

#endif
