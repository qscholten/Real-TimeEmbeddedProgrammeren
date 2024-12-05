//
//  verwerker.hpp
//  sensoren
//
//  Created by John Vi on 4/26/19.
//  Copyright © 2019 John Vi. All rights reserved.
//

#ifndef verwerker_hpp
#define verwerker_hpp

#include <stdio.h>
#include <vector>
#include <atomic>
using namespace std;

class Buffer;
class Verwerker {
public:
    Verwerker(Buffer*,int);
    void verwerkData();
    void zetUit();
    int hetResultaat()const;
    void aantalTeLezenData(int);
    vector<int> deTussenstanden() const;
private:
    static vector<int> tussenstanden;
    int hoeveel;
    Buffer *buf;
    static atomic<int> result;
    bool status;
    mutex m1;
};
#endif /* verwerker_hpp */
