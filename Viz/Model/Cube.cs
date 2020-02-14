using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viz.Model
{
    class Cube : Cell
    {
        List<Point> points;

        //----------------------------------------------Contstructors
        public Cube(Point[] points){
            this.points = points.ToList();
        }


        //----------------------------------------------Interface implementation
        public void addPoint(Point point)
        {
            this.points.Add(point);
        }
        public Point[] getPoints()
        {
            return points.ToArray();
        }
        public void setPoints(Point[] points)
        {
            this.points = points.ToList();
        }
        }
    }
}
