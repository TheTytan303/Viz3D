using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viz.Model
{

    /*  */
    interface Cell
    {
        /* Add point and set it as a last point */
        void addPoint(Point3D point);
        /* set points - replace current with those */
        void setPoints(Point3D[] points);
        /* get all points */
        Point3D[] getPoints();
    }
}
