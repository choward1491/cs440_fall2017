//
//  FileWrap.hpp
//
//  Created by Christian J Howard on 12/12/15.
//
//

#ifndef FileWrap_hpp
#define FileWrap_hpp

#include <stdio.h>
#include <string>

namespace wrap {

class file {
public:
    
    enum AccessMode { Read=0, Write, Append, ReadBinary, WriteBinary, AppendBinary, ReadUpdate, WriteUpdate, AppendUpdate };
    
    file();
    file( const char* filename , AccessMode mode );
    file( const std::string & filename, AccessMode mode );
    ~file();
    
    bool isOpen() const;
    void openFile( const char* filename, AccessMode mode );
    void openFile( const std::string & filename, AccessMode mode );
    void resetFile();
    
    operator FILE* () const;
    FILE* ref() const;
    
private:
    void clear();
    FILE* fp;
    
};

}

#endif /* FileObject_hpp */
