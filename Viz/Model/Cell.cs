using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viz.Model
{
    interface Cell
    {
        void addPoint(Point point);
        void setPoints(Point[] points);
        Point[] getPoints();
    }
}
