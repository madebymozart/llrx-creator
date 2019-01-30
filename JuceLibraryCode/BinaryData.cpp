/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== llrx.fbs ==================
static const unsigned char temp_binary_data_0[] =
"// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved\r\n"
"\r\n"
"table Llrx {\r\n"
"  amount:int;\r\n"
"  name:string;\r\n"
"  entities:[Entity];   // Vector of entities.\r\n"
"}\r\n"
"\r\n"
"table Entity {\r\n"
"  note:short;\r\n"
"  time:float;\r\n"
"  delta:float;\r\n"
"}\r\n"
"\r\n"
"root_type Llrx;";

const char* llrx_fbs = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xed70233e:  numBytes = 248; return llrx_fbs;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "llrx_fbs"
};

const char* originalFilenames[] =
{
    "llrx.fbs"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
