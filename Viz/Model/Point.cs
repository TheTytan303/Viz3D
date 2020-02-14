using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viz.Model
{
    class Point
    {
        protected double x, y, z;
        protected uint id;
        protected static uint ids =0;

        public Point(double x, double y, double z){
            this.x = x;
            this.y = y;
            this.z = z;
            this.id = ids++;
        }
    }
}
