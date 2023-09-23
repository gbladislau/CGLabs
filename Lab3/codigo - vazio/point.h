class Point2D
{
private:
    double x;
    double y;

    void setX(double x){this->x = x;}
    void setY(double y){this->y = y;}
public:
    Point2D(double x, double y){
        this->x = x;
        this->y = y;
    }
    Point2D(){
        this->x = 0.0;
        this->y = 0.0;
    }

    double getX(){ return this->x; }
    double getY(){ return this->y; }

    void RotateZ(double angle);

    void Translate (double dx, double dy);
};
