using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viz.Model
{
    class Cube : Cell
    {
        List<Point3D> points;

        //----------------------------------------------Contstructors
        public Cube(Point3D[] points){
            this.points = points.ToList();
        }


        //----------------------------------------------Interface implementation
        public void addPoint(Point3D point)
        {
            this.points.Add(point);
        }
        public Point3D[] getPoints()
        {
            return points.ToArray();
        }
        public void setPoints(Point3D[] points)
        {
            this.points = points.ToList();
        }
        }
    }
}
