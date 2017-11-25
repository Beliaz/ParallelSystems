#ifndef PARALLELSYSTEMSTEAM_BASEDEF_H
#define PARALLELSYSTEMSTEAM_BASEDEF_H

#endif //PARALLELSYSTEMSTEAM_BASEDEF_H

#define TYPE float
#define SIZETYPE long

TYPE* initArray(SIZETYPE size, short dimension) {
    TYPE *array = nullptr;
    switch (dimension) {
        default:
        case 1:
            array = new TYPE[size + 2];
            break;
        case 2:
            array = new TYPE[(size+2)*(size+2)];
            break;
        case 3:
            array = new TYPE[(size+2)*(size+2)*(size+2)];
            break;
    }
    return array;
}