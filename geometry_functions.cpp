#include <math.h>
#include <iomanip>
#include "objects.h"
#include <fstream>
using namespace std;

// ---------------------------------------------------------------

int World::round(float f) {
    int F = int(f);
    float diff = f - F;

    if (diff >= 0.5) return int(F + 1); // Positive number
    else return int(F); // Negative number
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

float World::round_decimalPlaces(float myValue, float PRECISION) {

    //for 2.d.p set precision to 0.01 and 4dp to 0.0001

    double myRemainder = fmod(myValue, PRECISION);

    if (myRemainder > (0.5 * PRECISION)) {
        myValue = (myValue - myRemainder + PRECISION);
    } else {
        myValue = (myValue - myRemainder);
    }

    return (myValue);
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
float World::get_perp_distance_from_point_to_line(Coordinates P1, Coordinates P2, MemAgent* mp){

    //P1 and P2 are start and end points of line, finding the point at which mp intersects this line such that
    //get closest pt on line to agent
    Coordinates* D = new Coordinates();
    Coordinates* Ves = new Coordinates();
    Coordinates* Closest = new Coordinates();
    float scalar;

    Ves->x = P2.x-P1.x;
    Ves->y = P2.y-P1.y;
    Ves->z = P2.z-P1.z;

     D->x = mp->Mx-P1.x;
    D->y = mp->My-P1.y;
    D->z = mp->Mz-P1.z;

    scalar = dotProduct(D, Ves)/dotProduct(Ves, Ves);

    Closest->x = P1.x+(scalar*Ves->x);
    Closest->y = P1.y+(scalar*Ves->y);
    Closest->z = P1.z+(scalar*Ves->z);

    //calc distance
    return(getDist(Closest->x, Closest->y, Closest->z, mp->Mx, mp->My, mp->Mz));
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
float World::getDist(float x1, float  y1, float z1, float x2, float y2, float z2) {

    float distance;
    distance=sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2))+((z1-z2)*(z1-z2)));

    return(distance);

}
//------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool World::distTest2D(int x1, int y1, int x2, int y2, int dist) {
    if(round(sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2))))<=dist) {
        return(true);
    }
    else {
        return(false);
    }
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
bool World::distTest2Dexact(float x1, float y1, float x2, float y2, float dist) {
    if(sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)))==dist) {
        return(true);
    }
    else {
        return(false);
    }
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
float World::dotProduct(Coordinates* first, Coordinates* second){

    return((first->x*second->x)+(first->y*second->y)+(first->z*second->z));
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
float World::absVal(Coordinates vec){

    return(sqrt((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z)));
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
float World::calc_z_given_equ_plane(float X, float Y, vector <float> coeffs){

    float Z;
    Z = (-(coeffs[0]*X)-(coeffs[1]*Y)-(coeffs[3]))/coeffs[2];

    return(Z);
}
//---------------------------------------------------------------------------------------------
//find equation of line through two points output the gradient
float World::equation_of_line_through_two_points_given_Coord(float x1, float y1, float x2, float y2, float A, int which) {

    float m, c, B;

    if((x2-x1)!=0){
        m=((float)y2-(float)y1)/((float)x2-(float)x1);
        c= y1 - m*x1;
    }
    else{
        m=0;
        c=y1;
    }

    //given coord is x
    if(which==0)
        B = m*A+c;
    //given coord is y
    else
        B = (A-c)/m;

    return(B);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//find equation of line through two points output the gradient
vector <float> World::equation_of_line_through_two_points(float x1, float y1, float x2, float y2) {

    float m, c;
    vector <float> ans;
    int flag=0;

    if((x2-x1)!=0){
        m=((float)y2-(float)y1)/((float)x2-(float)x1);
        c= y1 - m*x1;
    }
    else{
        //need to swap around = so its x = my+c, as the x's are the same so equation is flipped to get line x=c
        m=0;
        c=x1;
        flag=1;
    }

    //if size of ans = 3 then reverse equation so have x = my+c
    ans.push_back(m);
    ans.push_back(c);
    if(flag==1) ans.push_back(5.0f);

    return(ans);
}
//---------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
Coordinates World::findMidPoint(float x1, float y1, float z1, float x2, float y2, float z2, float Length){

    int k;
    float denom;
    float Coord[3];
    Coordinates coord;
    float PN[3];
    float P[3] = {x1, y1, z1};
    PN[0] = x1-x2;
    PN[1] = y1-y2;
    PN[2] = z1-z2;

    if(Length!=0){
        //toroidal adjustments
        if(sqrt(PN[0]*PN[0])>=(float)xMAX/2.0f){

            if(PN[0]>0) PN[0]= (x1-xMAX)-x2;//-((float)xMAX- PN[0]);
            else PN[0]=x1-(x2-xMAX);//(float)xMAX- fabs(PN[0]);

        }

        denom = sqrt((PN[0]*PN[0])+(PN[1]*PN[1])+(PN[2]*PN[2]));

        for(k=0;k<3;k++){
            Coord[k]=P[k]-((Length/2.0f)*(PN[k]/denom));
        }

        coord.x = Coord[0];
        coord.y = Coord[1];
        coord.z = Coord[2];

        if(coord.x<0) coord.x+=xMAX;
        else if(coord.x>=xMAX){
             coord.x-=xMAX;
             //somehow its sometimes letting a coord.x at xAMX get through.. added quick fix in new Nodes for now. 
        }
    }
    else{
        coord.x = x1;
        coord.y = y1;
        coord.z = z1;
        // cout<<"length 0 "<<coord.x<<" "<<coord.y<<" "<<coord.z<<" "<<x1<<" "<<y1<<" "<<z1<<endl;
    }



    return(coord);
}
//-------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
float World:: calcCurvature(Coordinates One, Coordinates Two, Coordinates Three){

    //calc curve equation between pairs of pairs
    float denom2, A1, B1, C1, R;
    float Sx, Sy, Fx, Fy, curvature;

    denom2 = ((One.x-Three.x)*((Two.x*Two.x)-(One.x*One.x)))+((Two.x-One.x)*((Three.x*Three.x)-(One.x*One.x)));

    A1 = ((Two.y - One.y)*(One.x-Three.x)+(Three.y - One.y)*(Two.x-One.x))/denom2;

    B1 = ((Two.y-One.y)-A1*((Two.x*Two.x)-(One.x*One.x)))/(Two.x-One.x);

    C1 = One.y - A1*(One.x*One.x)-(B1*One.x);

    //cout<<"coeffs: "<<A1<<" "<<B1<<" "<<C1<<endl;

    //calc stationary point - where first diff is 0
    Sx = -B1/A1;
    Sy = A1*(Sx*Sx) + B1*Sx+ C1;

    //calc focus point of circle which lies on the curve at stationary point
    Fx = -B1/(2*A1);
    Fy = -((B1*B1)/(4*A1))+C1+(1/(4*A1));

    //radius of curvature os the distance between focus of cirle and stationary pt.
    curvature = getDist(Sx, Sy, 0.0, Fx, Fy, 0.0);

    float dy;
    float top;

    dy = (2*A1*Sx)+B1;
    top = (1+(dy*dy));

    R = (top*sqrt(top))/(2*A1); //see book 4 pp-6
    //cout<<"R = "<<R<<endl;
    return(curvature);

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
vector <Coordinates> World::initialisePoints(void){

    ifstream points;
    points.open("points.txt");
    float in;
    //char curvename[7];
    //Coord curvename;
    Coordinates currentCurve;
    int counter=0;
    vector <Coordinates> curves;

    do{
        points >> in;
        if(counter%2==0){
            //sprintf(curvename, "curve%d", num);
            //num++;
            if(counter!=0) curves.push_back(currentCurve);

            currentCurve.x = in;

        }
        else if(counter%2==1){
            currentCurve.y = in;

            currentCurve.z=0.0f;
        }
        //cout<<"counter";
        counter++;

    }while(!points.eof());

    //curves.push_back(currentCurve);

    points.close();

    return(curves);

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
void World::calcCurves(void){

    vector <Coordinates> curves;
    int i;
    float curvature;
    curves = initialisePoints();

    for(i=0;i<(int)curves.size();i++){
        cout<<curves[i].x<<" "<<curves[i].y<<" "<<endl;

    }

    cout<<endl;
    for(i=0;i<(float)curves.size()/3.0;i++){
        curvature = calcCurvature(curves[0 +(i*3)], curves[1+(i*3)], curves[2+(i*3)]);
        cout<<"curvature: "<<curvature<<endl;
    }

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
float* World::equation_of_plane_through_three_points(vector<Coordinates> pts){

    float A, B, C, D, D1, D2, D3;
    float* coeffs = new float[4];


    //equation of plane given by Ax+By+Cz+D = 0;
    //if for a given point x,y,z, when plugged into this equation gives a positive number then that point
    //is not on the plane but on the same side as the normal to the plane, -ve => opp side.

    A = (pts[0].y*(pts[1].z-pts[2].z))+(pts[1].y*(pts[2].z-pts[0].z))+(pts[2].y*(pts[0].z-pts[1].z));
    B = (pts[0].z*(pts[1].x-pts[2].x))+(pts[1].z*(pts[2].x-pts[0].x))+(pts[2].z*(pts[0].x-pts[1].x));
    C = (pts[0].x*(pts[1].y-pts[2].y))+(pts[1].x*(pts[2].y-pts[0].y))+(pts[2].x*(pts[0].y-pts[1].y));
    D1 = (pts[0].x*((pts[1].y*pts[2].z)-(pts[2].y*pts[1].z)));
    D2 = (pts[1].x*((pts[2].y*pts[0].z)-(pts[0].y*pts[2].z)));
    D3 = (pts[2].x*((pts[0].y*pts[1].z) - (pts[1].y*pts[0].z)));
    D =-(D1+D2+D3);

    coeffs[0] = A;
    coeffs[1] = B;
    coeffs[2] = C;
    coeffs[3] = D;

    return(coeffs);


}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
bool World::check_point_on_plane(vector <float> coeffs, Coordinates point){

    if((coeffs[0]*point.x)+(coeffs[1]*point.y)+(coeffs[2]*point.z)+(coeffs[3])==0) return(true);
    else return(false);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
Coordinates* World::crossProduct(Coordinates* one, Coordinates* two){

    Coordinates* ans = new Coordinates();

    ans->x = (one->y*two->z)-(one->z*two->y);
    ans->y = (one->z*two->x)-(one->x*two->z);
    ans->z = (one->x*two->y)-(one->y*two->x);

    return(ans);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
bool World::check_point_same_side_of_line(Coordinates* point, Coordinates* order, bool hyp){

    //cross product of both points with line, when dotted together should = 0 if on same side
    Coordinates* first;
    Coordinates* second;
    Coordinates* AB= new Coordinates();
    Coordinates* PA = new Coordinates();
    Coordinates* CPA= new Coordinates();
    bool ans;

    AB->x = order[1].x-order[2].x;
    AB->y = order[1].y-order[2].y;
    AB->z = order[1].z-order[2].z;

    CPA->x = order[0].x-order[1].x;
    CPA->y = order[0].y-order[1].y;
    CPA->z = order[0].z-order[1].z;

    PA->x = point->x-order[1].x;
    PA->y = point->y-order[1].y;
    PA->z = point->z-order[1].z;

    first = crossProduct(AB, PA);
    second = crossProduct(AB, CPA);

    delete AB;
    delete PA;
    delete CPA;

    if(hyp==true) {
        if(dotProduct(first, second)>=0) ans = true;

        else ans = false;
    }
    else{
        if(dotProduct(first, second)>0) ans = true;
        else ans = false;
    }

    delete first;
    delete second;

    return(ans);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
bool World::check_point_within_triangle(Coordinates* point, vector<Coordinates> triangle){

    //if its the hypotenuse of the triange, the one line that isnt actually a spring, but the divider of the 4 way mesh into
    //two triangles, then need to test for points on that line aswell as within the triangle.
    Coordinates* order = new Coordinates[3];



        order[0] = triangle[1];
            order[1] = triangle[0];
            order[2] = triangle[2];
    if(check_point_same_side_of_line(point, order, true)==true){
         order[0] = triangle[2];
        order[1] = triangle[0];
        order[2] = triangle[1];
        if(check_point_same_side_of_line(point, order, true)==true){
        order[0] = triangle[0];
    order[1] = triangle[1];
    order[2] = triangle[2];

            if(check_point_same_side_of_line(point, order, true)==true){
                delete[] order;
                return(true);
            }
            else{
            delete[] order;
            return(false);
            }
        }
        else{
        delete[] order;
        return(false);
        }
    }
    else{
    delete[] order;
     return(false);
     }

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
int* World::findRange(vector <Coordinates> coords){

    int smallest, largest;
    int* range = new int[6];

    //get smallest x and largest x, y,z of the three coords
    //to then use as the block of points to test if they are on plane and in triangel
    //-----------------------------------------------------------------------
    //cout<<coords[0].x<<endl;
    if(coords[0].x<=coords[1].x){

        //smallest
        if(coords[0].x<=coords[2].x){
            smallest = (int)coords[0].x;
        }
        else{
            smallest = (int)coords[2].x;
        }

        //largest
        if(coords[1].x>=coords[2].x){
            largest = (int)coords[1].x;
        }
        else{
            largest = (int)coords[2].x;
        }

    }
    else{
        //smallest
        if(coords[1].x<=coords[2].x){
            smallest = (int)coords[1].x;
        }
        else{
            smallest = (int)coords[2].x;
        }

        //largest
        if(coords[0].x>=coords[2].x){
            largest = (int)coords[0].x;
        }
        else{
            largest = (int)coords[2].x;
        }

    }

    range[0] = smallest;
    range[1] = largest;
    //-----------------------------------------------------------------------
    if(coords[0].y<=coords[1].y){

        //smallest
        if(coords[0].y<=coords[2].y){
            smallest = (int)coords[0].y;
        }
        else{
            smallest = (int)coords[2].y;
        }

        //largest
        if(coords[1].y>=coords[2].y){
            largest = (int)coords[1].y;
        }
        else{
            largest = (int)coords[2].y;
        }

    }
    else{
        //smallest
        if(coords[1].y<=coords[2].y){
            smallest = (int)coords[1].y;
        }
        else{
            smallest = (int)coords[2].y;
        }

        //largest
        if(coords[0].y>=coords[2].y){
            largest = (int)coords[0].y;
        }
        else{
            largest = (int)coords[2].y;
        }

    }
    range[2] = smallest;
    range[3] = largest;

    //-----------------------------------------------------------------------
    if(coords[0].z<=coords[1].z){

        //smallest
        if(coords[0].z<=coords[2].z){
            smallest = (int)coords[0].z;
        }
        else{
            smallest = (int)coords[2].z;
        }

        //largest
        if(coords[1].z>=coords[2].z){
            largest = (int)coords[1].z;
        }
        else{
            largest = (int)coords[2].z;
        }

    }
    else{
        //smallest
        if(coords[1].z<=coords[2].z){
            smallest = (int)coords[1].z;
        }
        else{
            smallest = (int)coords[2].z;
        }

        //largest
        if(coords[0].z>=coords[2].z){
            largest = (int)coords[0].z;
        }
        else{
            largest = (int)coords[2].z;
        }

    }
    range[4] = smallest;
    range[5] = largest;

    /*int i;
    for(i=0;i<3;i++)
        cout<<"coords "<<i<<": "<<coords[i].x<<" "<<coords[i].y<<" "<<coords[i].z<<" "<<endl;
    for(i=0;i<6;i++)
    cout<<"range "<<range[i]<<" ";
*/
   // cout<<endl;

    //----------------------------------------------------------------------
    return(range);

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
void World::trianglePlaneTest(void){

    /*
    //needs changing as now passing array pointers not vectors...

    int i, j, k;
    Coordinates one;
    Coordinates two;
    Coordinates three;
    Coordinates point;
    bool onPlane;
    bool inTriangle;
    int* range;

    //always make coordinates one and two the two that are being joined by an invisible hypotenuse for the triangle,
    //they are the only ones that actually do not have a spring between them.

    one.x = 1.0f;
    one.y = 1.0f;
    one.z = 0.0f;

    two.x = 2.5f;
    two.y = 4.0f;
    two.z = 0.0f;

    three.x = 4.0f;
    three.y = 1.0f;
    three.z = 0.0f;

    float* coeffs;

    Coordinates* coords = new Coordinates[3];
    coords[0] =one;
    coords[1] =two;
    coords[2] =three;

    range = findRange(coords);

    coeffs = equation_of_plane_through_three_points(one, two, three);

    for(i=range[0];i<=range[1];i++){
        for(j=range[2];j<=range[3];j++){
            for(k=range[4];k<=range[5];k++){

                point.x = i;
                point.y = j;
                point.z = k;

                onPlane = check_point_on_plane(coeffs, point);
                if(onPlane==true) inTriangle = check_point_within_triangle(point, one, two, three);
                //if(inTriangle==true) cout<<"TR: "<<i<<" "<<j<<" "<<k<<endl;
            }
        }
    }

    delete[] range;
    delete[] coords;*/

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//intersection of line with plane - if denom is zero they dont intersect
float World::find_denom(Coordinates one, Coordinates two, float* coeffs){

    float denom = coeffs[0]*(two.x-one.x)+ coeffs[1]*(two.y-one.y)+ coeffs[2]*(two.z-one.z);

    return(denom);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
Coordinates* World::pt_of_intersection_plane_line(Coordinates pt_one, Coordinates pt_two, float* plane_coeffs){
    int i;



    Coordinates* intersect_pt;



    float num, t;

    float denom = find_denom(pt_one, pt_two, plane_coeffs);

    if(denom!=0.0f){

        //find t
        //have plugged parametric form of line into plane equation, as when they intersect that pt in plane equ =0,
        //rearranged to get t
        //Ax + By+ Cz+ D = 0  - the plane equ
        //t = Ax1_By1+Cz1+D/denom

        num = (plane_coeffs[0]*pt_one.x)+(plane_coeffs[1]*pt_one.y)+(plane_coeffs[2]*pt_one.z)+plane_coeffs[3];

        t = -num/denom;

        //cout<<"t = "<<t<<endl;
        //plug t back into parametric form of line equation
        //x = x1+(x2-x1)t
        //y = y1+(y2-y1)t
        //z = z1+(z2-z1)t
        intersect_pt= new Coordinates();

        intersect_pt->x = pt_one.x+(pt_two.x*t)-(pt_one.x*t);
        intersect_pt->y = pt_one.y+(pt_two.y*t)-(pt_one.y*t);
        intersect_pt->z = pt_one.z+(pt_two.z*t)-(pt_one.z*t);


    }
    else intersect_pt=NULL;
    //if denom is zero then they dont intersect and thus retruns an empty vector

    return(intersect_pt);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
bool World::check_point_within_cube(Coordinates* point, float x, float y, float z){

    if((point->x>=x-0.5)&&(point->x<=x+0.5)){
        if((point->y>=y-0.5)&&(point->y<=y+0.5)){
            if((point->z>=z-0.5)&&(point->z<=z+0.5)){
                return(true);
            }
            else return(false);
        }
        else return(false);
    }
    else return(false);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
bool World::trianglePlane_cubeDiagonals_intersect_test(vector<Coordinates> triangle, float x, float y, float z, Coordinates* cube_vertex){

    int i=0;
    bool intersect = false;
    Coordinates* point =NULL;
    float* coeffs;
    bool inTriangle;
    bool inCube;

    coeffs = equation_of_plane_through_three_points(triangle);
    //cout<<coeffs[0]<<" "<<coeffs[1]<<" "<<coeffs[2]<<" "<<coeffs[3]<<" "<<endl;

    //given an integer cube where x,y,z is the centre;

    //for each of the cube diagonals (see page 28 book 4) vertices labelled one less as starts from 0 not 1
    //test if it intersects the triangle plane
    do{
        if(i==0){
            point = pt_of_intersection_plane_line(cube_vertex[0], cube_vertex[6], coeffs);
        }
        else if(i==1){
            point = pt_of_intersection_plane_line(cube_vertex[1], cube_vertex[7], coeffs);
        }
        else if(i==2){
            point = pt_of_intersection_plane_line(cube_vertex[2], cube_vertex[4], coeffs);
        }
        else{
            point = pt_of_intersection_plane_line(cube_vertex[3], cube_vertex[5], coeffs);
        }

        //if it does check if that point is in the triangle and in the cube
        if(point!=NULL){

            //cout<<"got a point "<<point[0].x<<" "<<point[0].y<<" "<<point[0].z<<endl;
             inCube = check_point_within_cube(point, x, y, z);

            if(inCube==true){
              inTriangle = check_point_within_triangle(point, triangle);
              if(inTriangle==true) intersect = true;
            }
            delete point;
            //if not continue testing against diagonals, otherwise stop
        }
        i++;
    }while((i<4)&&(intersect==false));

    delete[] coeffs;
    return(intersect);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
void World::test_plane_line_intersect(void){

    /*        bool intersect;
     * vector <Coordinates> triangle;
     * Coordinates one;
     * Coordinates two;
     * Coordinates three;
     * float x,y,z;
     *
     * x = 0.5;
     * y = 0.5;
     * z = 0.5;
     *
     * one.x = 0.6f;
     * one.y = 0.8f;
     * one.z = 0.0f;
     *
     * two.x = 10.0f;
     * two.y = 0.5f;
     * two.z = 0.0f;
     *
     * three.x = 1.3f;
     * three.y = 0.5f;
     * three.z = 1.0f;
     *
     * triangle.push_back(one);
     * triangle.push_back(two);
     * triangle.push_back(three);
     *
     * intersect = trianglePlane_cubeDiagonals_intersect_test(triangle, x, y, z);
     *
     * if(intersect==true) cout<<"cube triangle intersect!"<<endl;
     * else cout<<"no intersection"<<endl;*/
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
Coordinates* World::get_cube_vertices(float x, float y, float z){

    Coordinates vertex;
    Coordinates* cube_vertices = new Coordinates[8];

    vertex.x = x-0.5; vertex.y = y+0.5; vertex.z = z-0.5; cube_vertices[0] = vertex;
    vertex.x = x+0.5; vertex.y = y+0.5; vertex.z = z-0.5; cube_vertices[1] =vertex;
    vertex.x = x+0.5; vertex.y = y+0.5; vertex.z = z+0.5; cube_vertices[2] = vertex;
    vertex.x = x-0.5; vertex.y = y+0.5; vertex.z = z+0.5; cube_vertices[3] = vertex;
    vertex.x = x-0.5; vertex.y = y-0.5; vertex.z = z-0.5; cube_vertices[4] = vertex;
    vertex.x = x+0.5; vertex.y = y-0.5; vertex.z = z-0.5; cube_vertices[5] = vertex;
    vertex.x = x+0.5; vertex.y = y-0.5; vertex.z = z+0.5; cube_vertices[6] = vertex;
    vertex.x = x-0.5; vertex.y = y-0.5; vertex.z = z+0.5; cube_vertices[7] = vertex;

    return(cube_vertices);

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
void World::voxelisation(void){

   /* vector <Coordinates> triangle;
    bool intersect;
    triangle = initialise_test_triangle();

    //test just with one cube first, then do with all range of cubes with get range
    float x, y, z;

    x = 0.5;
    y = 0.5;
    z = 0.5;

    vector <Coordinates> cube_vertex = get_cube_vertices(x, y, z);

    //first: test if triangle lines intersect cube face planes
    intersect = triangleLines_cubeFacePlane_intersect_test(triangle, x, y, z, cube_vertex);
    //if not test if cube diagonal lines intersect triangle plane.
    if(intersect==true) cout<<"triangle lines faces intersect!"<<endl;

    if(intersect==false)
        intersect = trianglePlane_cubeDiagonals_intersect_test(triangle, x, y, z, cube_vertex);

    //if(intersect==true) cout<<"cube triangle intersect!"<<endl;
    //else cout<<"no intersection"<<endl;*/
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
bool World::voxelise(vector <Coordinates> triangle, float x, float y, float z){

    bool intersect=false;


    //Coordinates* cube_vertex;
    //cube_vertex = get_cube_vertices(x, y, z);

    Coordinates vertex;
    Coordinates* cube_vertices = new Coordinates[8];

    vertex.x = x-0.5; vertex.y = y+0.5; vertex.z = z-0.5; cube_vertices[0] = vertex;
    vertex.x = x+0.5; vertex.y = y+0.5; vertex.z = z-0.5; cube_vertices[1] =vertex;
    vertex.x = x+0.5; vertex.y = y+0.5; vertex.z = z+0.5; cube_vertices[2] = vertex;
    vertex.x = x-0.5; vertex.y = y+0.5; vertex.z = z+0.5; cube_vertices[3] = vertex;
    vertex.x = x-0.5; vertex.y = y-0.5; vertex.z = z-0.5; cube_vertices[4] = vertex;
    vertex.x = x+0.5; vertex.y = y-0.5; vertex.z = z-0.5; cube_vertices[5] = vertex;
    vertex.x = x+0.5; vertex.y = y-0.5; vertex.z = z+0.5; cube_vertices[6] = vertex;
    vertex.x = x-0.5; vertex.y = y-0.5; vertex.z = z+0.5; cube_vertices[7] = vertex;

    //first: test if triangle lines intersect cube face planes
    //if(samePlane==false)
    intersect = trianglePlane_cubeDiagonals_intersect_test(triangle, x, y, z, cube_vertices);

    //if(intersect==false)
      //intersect = triangleLines_cubeFacePlane_intersect_test(triangle, x, y, z, cube_vertices);
    //}


    delete[] cube_vertices;


    return(intersect);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
vector <Coordinates> World::initialise_test_triangle(void){

    vector <Coordinates> triangle;
    Coordinates one;
    Coordinates two;
    Coordinates three;


    one.x = 0.0f;
    one.y = 1.6f;
    one.z = 0.1f;

    two.x = 0.5f;
    two.y = 0.9f;
    two.z = 0.1f;

    three.x = 0.8f;
    three.y = 1.6f;
    three.z = 0.1f;

    triangle.push_back(one);
    triangle.push_back(two);
    triangle.push_back(three);

    return(triangle);
}
//---------------------------------------------------------------------------------------------
bool World::check_point_in_line(Coordinates* point, Coordinates start, Coordinates fin){

    int flag=0;
    int flag1=0;
    int flag2=0;
    float err = 0.000001;

    if(start.x<=fin.x){
        if((point->x>=start.x)&&(point->x<=fin.x))flag=1;
        //check its not just a problem with the calculated point coordinate being wrong due to not rounding to less decimal
        //places
        else if((fabs(point->x-start.x)<=err)||(fabs(point->x-fin.x)<=err))flag=1;

    }
    else{
        if((point->x>=fin.x)&&(point->x<=start.x)) flag=1;
        else if((fabs(point->x-start.x)<=err)||(fabs(point->x-fin.x)<=err))flag=1;
    }

    if(flag==1){
        if(start.y<=fin.y){
            if((point->y>=start.y)&&(point->y<=fin.y)) flag1=1;
            else if((fabs(point->y-start.y)<=err)||(fabs(point->y-fin.y)<=err)) flag1=1;
        }
        else{
            if((point->y>=fin.y)&&(point->y<=start.y)) flag1=1;
            else if((fabs(point->y-start.y)<=err)||(fabs(point->y-fin.y)<=err)) flag1=1;
        }
        if(flag1==1){

            if(start.z<=fin.z){
                if((point->z>=start.z)&&(point->z<=fin.z)) flag=1;

                else if((fabs(point->z-start.z)<=err)||(fabs(point->z-fin.z)<=err)) flag2=1;
            }
            else{
                if((point->z>=fin.z)&&(point->z<=start.z)) flag=1;
                else if((fabs(point->z-start.z)<=err)||(fabs(point->z-fin.z)<=err)) flag2=1;
            }
        }
    }

    if(flag2==1) return(true);
    else return(false);
}
//---------------------------------------------------------------------------------------------
bool World::triangleLines_cubeFacePlane_intersect_test(vector<Coordinates> triangle, float x, float y, float z, Coordinates* cube_vertex){

    Coordinates* point;
    vector<Coordinates> points;

    float* coeffs1;

    bool intersect=false;

    //vector < float* > coeffs;
    //get cube face planes


    points.push_back(cube_vertex[0]);
    points.push_back(cube_vertex[1]);
    points.push_back(cube_vertex[2]);


    //for each triangle line, try to intersect with plane of cube face
    int i=0;
    int j=0;
    int k=1;
    int m=0;



    do{
    	coeffs1 = equation_of_plane_through_three_points(points);

        point = pt_of_intersection_plane_line(triangle[j], triangle[k], coeffs1);

        delete[] coeffs1;


        if(point!=NULL){

            //cout<<point[0].x<<" "<<point[0].y<<" "<<point[0].z<<endl;
            intersect = check_point_within_cube(point, x, y, z);
            if(intersect==true) intersect = check_point_in_line(point, triangle[j], triangle[k]);
            delete point;
        }

        i++;
        m++;
        if(m==6){
            i=0;
            j=1;
            k=2;
        }
        else if(m==12){
            i=0;
            j=0;
            k=2;
        }
        if((i==1)&&(m<6)){
            points[0] = cube_vertex[4];
    	    points[1] = cube_vertex[5];
    	    points[2] = cube_vertex[6];


        }
        else if((i==2)&&(m<6)){
            points[0] = cube_vertex[3];
    	    points[1] = cube_vertex[0];
    	    points[2] = cube_vertex[4];


        }
        else if((i==3)&&(m<6)){
            points[0] = cube_vertex[0];
    	    points[1] = cube_vertex[1];
    	    points[2] = cube_vertex[5];


        }
        else if((i==4)&&(m<6)){
            points[0] = cube_vertex[1];
    	    points[1] = cube_vertex[2];
    	    points[2] = cube_vertex[5];


        }
        else if((i==5)&&(m<6)){
            points[0] = cube_vertex[2];
    	    points[1] = cube_vertex[3];
    	    points[2] = cube_vertex[7];


        }

    }while((intersect==false)&&(m<18));

    points.clear();
    return(intersect);
}
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
Coordinates World::calc_point_on_line_through_external_point_giving_perpendicular_line(Coordinates C, Coordinates S, Coordinates En){

    vector <float> ans;
    Coordinates D;
    //line SE, point D is on line SE and gives a perpendicular line through external point C

    //dot product of CD.SE=0 is perpendicular
    //Dy=mDx+C for m and c of line SE if point is on the line
    //so by substitution find Dx and Dy.

    ans = equation_of_line_through_two_points(S.x, S.y, En.x, En.y);

    if(ans.size()==3){
        //then its a straight line in x axis so do x = my+c

        if(ans[0]==0){
            D.x = ans[1];

            if(En.y-S.y!=0)
                D.y = (((En.y-S.y)*C.y)-(D.x*(En.x-S.x))+(C.x*(En.x-S.x)))/(En.y-S.y);//-(((D.x-C.y)*(En.y-S.y))/(En.x-S.x))+C.x;
            else
                D.y = 0;

        }
        else{
          cout<<"oh it appears you can get a reverse equation of line when m isnt 0"<<endl;
        }
    }
    else{

       //then treat as normal
       //Dy = (m*Dx)+c;

        //can jump ahead if m=0
        if(ans[0]==0){
            D.y = ans[1];

            D.x = -(((D.y-C.y)*(En.y-S.y))/(En.x-S.x))+C.x;
        }
        else{
            //Dy = mDx+c
            //substitute into vec equ..
            //(Dx-Cx)*SEx+((mDx+c)-Cy)*SEy=0
            //Dx =( -(((mDx+c)-Cy)*SEy)/SEx)+Cx

            D.x = (((En.x-S.x)*C.x) +((En.y-S.y)*(C.y-ans[1])))/((En.x-S.x)+((En.y-S.y)*ans[0]));
            D.y = (ans[0]*D.x)+ans[1];
        }
       //So,

    }

    D.z = 0.0f;
    return(D);




}
//----------------------------------------------------------------------------------------------------------
