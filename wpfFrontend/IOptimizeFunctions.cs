using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace IOptimizeWPFFrontend
{
    static public class IOptimizeFunctions 
    {
        [Flags]
        public enum IOptimizeTypeFlags
        {
            None = 0x00000000,
            Revert = 0x00000001,
            FPS = 0x00000002,
            Latency = 0x00000004,
            // Must also add IOPTIMIZE_TYPE_FPS flag bit when using this flag bit
            Fortnite = 0x00000008,
            // Must also add IOPTIMIZE_TYPE_FPS flag bit when using this flag bit
            ApexLegends = 0x00000010,
        }

        [DllImport("IOptimize.dll")]
        public static extern void IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType);

        [DllImport("IOptimize.dll")]
        public static extern void IOptimizeSetDisplayResolution(int width, int height);
    }
}
