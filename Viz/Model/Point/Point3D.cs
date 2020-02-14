using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viz.Model.Dimensions;

namespace Viz.Model
{
    class Point3D : Point<Extent3D>
    {
        protected double x, y, z;
        protected uint id;
        protected static uint ids =0;
        
        public Point3D(double x, double y, double z){
            this.x = x;
            this.y = y;
            this.z = z;
            this.id = ids++;
        }
    }
}
