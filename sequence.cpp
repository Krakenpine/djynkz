#include <vector>

struct SeqEvent {
    int tick;
    bool playNote;
    int note;
    bool stac;
};

int tempo = 100;
int seqLength = 80;


SeqEvent sequence666[] = {
    {0,  true,  33, false},
    {0,  true,  40, false},
    {0,  true,  45, false},
    {80,  false, 33, false},
    {80,  false, 40, false},
    {80,  false, 45, false}
};

SeqEvent sequence125[] = {
    {0,  true,  33, false},
    {0,  true,  40, false},
    {0,  true,  45, false},
    {8,  false, 33, false},
    {8,  false, 40, false},
    {8,  false, 45, false},

    {8,  true,  35, false},
    {8,  true,  42, false},
    {8,  true,  47, false},
    {16, false, 35, false},
    {16, false, 42, false},
    {16, false, 47, false},

    {16, true,  33, false},
    {16, true,  40, false},
    {16, true,  45, false},
    {24, false, 33, false},
    {24, false, 40, false},
    {24, false, 45, false},

    {24, true,  36, false},
    {24, true,  43, false},
    {24, true,  48, false},
    {28, false, 36, false},
    {28, false, 43, false},
    {28, false, 48, false},

    {28, true,  33, true},
    {28, true,  40, true},
    {28, true,  45, true},
    {30, false, 33, true},
    {30, false, 40, true},
    {30, false, 45, true},

    {30, true,  33, true},
    {30, true,  40, true},
    {30, true,  45, true},
    {32, false, 33, true},
    {32, false, 40, true},
    {32, false, 45, true},

    {32, true,  33, false},
    {32, true,  40, false},
    {32, true,  45, false},
    {40, false, 33, false},
    {40, false, 40, false},
    {40, false, 45, false},

    {40, true,  35, false},
    {40, true,  42, false},
    {40, true,  47, false},
    {48, false, 35, false},
    {48, false, 42, false},
    {48, false, 47, false},

    {48, true,  33, false},
    {48, true,  40, false},
    {48, true,  45, false},
    {56, false, 33, false},
    {56, false, 40, false},
    {56, false, 45, false},

    {56, true,  48, false},
    {60, false, 48, false},
    {60, true,  47, false},
    {64, false, 47, false}
};

SeqEvent sequence6[] = {
    {0,  true,  40, true}, // 0 x x x x x
    {1,  false, 40, true}, // x
    {2,  true,  40, true}, // 0 x x x x x
    {3,  false, 40, true}, // x
    {4,  true,  52, false}, // x 7 x x x x
    {6,  false, 52, false}, // x
    {6,  true,  40, true}, // 0 x x x x x 
    {7,  false, 40, true}, // x
    {8,  true,  40, true}, // 0 x x x x x
    {9,  false, 40, true}, // x
    {10, true,  50, false}, // x 5 x x x x
    {12, false, 50, false}, // x
    {12, true,  40, true}, // 0 x x x x x
    {13, false, 40, true}, // x
    {14, true,  40, true}, // 0 x x x x x
    {15, false, 40, true}, // x
    {16, true,  48, false}, // x 3 x x x x
    {18, false, 48, false}, // x
    {18, true,  40, true}, // 0 x x x x x 
    {19, false, 40, true}, // x
    {20, true,  40, true}, // 0 x x x x x
    {21, false, 40, true}, // x
    {22, true,  46, false}, // x 1 x x x x
    {24, false, 46, false}, // x
    {24, true,  40, true}, // 0 x x x x x
    {25, false, 40, true}, // x
    {26, true,  40, true}, // 0 x x x x x
    {27, false, 40, true}, // x
    {28, true,  47, false}, // x 2 x x x x
    {30, false, 47, false}, // x
    {30, true,  48, false}, // x 3 x x x x
    {32, false, 48, false}, // x
    {32, true,  40, true}, // 0 x x x x x
    {33, false, 40, true}, // x
    {34, true,  40, true}, // 0 x x x x x
    {35, false, 40, true}, // x
    {36, true,  52, false}, // x 7 x x x x
    {38, false, 52, false}, // x
    {38, true,  40, true}, // 0 x x x x x 
    {39, false, 40, true}, // x
    {40, true,  40, true}, // 0 x x x x x
    {41, false, 40, true}, // x
    {42, true,  50, false}, // x 5 x x x x
    {44, false, 50, false}, // x
    {44, true,  40, true}, // 0 x x x x x
    {45, false, 40, true}, // x
    {46, true,  40, true}, // 0 x x x x x
    {47, false, 40, true}, // x
    {48, true,  48, false}, // x 3 x x x x
    {50, false, 48, false}, // x
    {50, true,  40, true}, // 0 x x x x x 
    {51, false, 40, true}, // x
    {52, true,  40, true}, // 0 x x x x x
    {53, false, 40, true}, // x
    {54, true,  46, false}, // x 1 x x x x
    {54, true,  40, false}, // 0 x x x x x
    {64, false, 40, false},
    {64, false, 46, false}
};

SeqEvent sequence3[] = {
    {0,  true,  33, true}, // 0 x x x x x
    {1,  false, 33, true}, // x
    {2,  true,  33, true}, // 0 x x x x x
    {3,  false, 33, true}, // x
    {4,  true,  45, false}, // x 7 x x x x
    {6,  false, 45, false}, // x
    {6,  true,  33, true}, // 0 x x x x x 
    {8,  false, 33, true}, // x
    {8,  true,  33, true}, // 0 x x x x x
    {10, false, 33, true}, // x
    {10, true,  43, false}, // x 5 x x x x
    {12, false, 43, false}, // x
    {12, true,  33, true}, // 0 x x x x x
    {14, false, 33, true}, // x
    {14, true,  33, true}, // 0 x x x x x
    {16, false, 33, true}, // x
    {16, true,  41, false}, // x 3 x x x x
    {18, false, 41, false}, // x
    {18, true,  33, true}, // 0 x x x x x 
    {20, false, 33, true}, // x
    {20, true,  33, true}, // 0 x x x x x
    {22, false, 33, true}, // x
    {22, true,  39, false}, // x 1 x x x x
    {24, false, 39, false}, // x
    {24, true,  33, true}, // 0 x x x x x
    {26, false, 33, true}, // x
    {26, true,  33, true}, // 0 x x x x x
    {28, false, 33, true}, // x
    {28, true,  40, false}, // x 2 x x x x
    {30, false, 40, false}, // x
    {30, true,  41, false}, // x 3 x x x x
    {32, false, 41, false}, // x
    {32, true,  33, true}, // 0 x x x x x
    {34, false, 33, true}, // x
    {34, true,  33, true}, // 0 x x x x x
    {36, false, 33, true}, // x
    {36, true,  45, false}, // x 7 x x x x
    {38, false, 45, false}, // x
    {38, true,  33, true}, // 0 x x x x x 
    {40, false, 33, true}, // x
    {40, true,  33, true}, // 0 x x x x x
    {42, false, 33, true}, // x
    {42, true,  43, false}, // x 5 x x x x
    {44, false, 43, false}, // x
    {44, true,  33, true}, // 0 x x x x x
    {46, false, 33, true}, // x
    {46, true,  33, true}, // 0 x x x x x
    {48, false, 33, true}, // x
    {48, true,  41, false}, // x 3 x x x x
    {50, false, 41, false}, // x
    {50, true,  33, true}, // 0 x x x x x 
    {52, false, 33, true}, // x
    {52, true,  33, true}, // 0 x x x x x
    {54, false, 33, true}, // x
    {54, true,  39, false}, // x 1 x x x x
    {54, true,  33, false}, // 0 x x x x x
    {64, false, 33, false},
    {64, false, 39, false}
};