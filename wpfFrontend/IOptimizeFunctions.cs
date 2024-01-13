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
        [StructLayout(LayoutKind.Explicit)]
        public struct IOptimizeVector2
        {
            // To simulate a union in C
            // Union 1
            [FieldOffset(0)] public float width;
            [FieldOffset(0)] public float x;
            // Union 2
            [FieldOffset(4)] public float height;
            [FieldOffset(4)] public float y;

        }


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

        // Parameter msi is to be dealt with like a boolean
        [DllImport("IOptimize.dll")]
        public static extern void IOptimizeSetGpuMsiMode(int msi);

        [DllImport("IOptimize.dll")]
        public static extern void IOptimizeSetDisplayResolution(int width, int height);
    }
}
