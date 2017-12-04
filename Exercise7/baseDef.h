#ifndef PARALLELSYSTEMSTEAM_BASEDEF_H
#define PARALLELSYSTEMSTEAM_BASEDEF_H

#endif //PARALLELSYSTEMSTEAM_BASEDEF_H

#define TYPE float
#define SIZETYPE long
#define epsilonStop 20


#ifndef DOPRINT
#define DOPRINT false
#endif

//Creates an array based on the dimension an size
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