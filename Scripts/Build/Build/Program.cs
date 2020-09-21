using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text;

namespace Build
{
    class Program
    {
        static void Main(string[] args)
        {
            Setup setup = new Setup();
            setup.Start();
        }

    }
}
