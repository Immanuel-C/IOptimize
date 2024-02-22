using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace IOptimizeWPFFrontend
{
    static public class IOptimize 
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

        public enum IOptimizeBool
        {
            True = 1,
            False = 0,
        }

        [DllImport("IOptimize.dll")]
        public static extern void IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType);

        // Parameter msi is to be dealt with like a boolean
        [DllImport("IOptimize.dll")]
        public static extern void IOptimizeSetGpuMsiMode(IOptimizeBool msi);

        [DllImport("IOptimize.dll")]
        public static extern void IOptimizeSetDisplayResolution(int width, int height);

        [DllImport("IOptimize.dll")]
        public static extern void IOptimizeSetTimerResolution(UInt32 resolutionMs);


        public struct TimerResolutionValues
        {
            public UInt32 maxResolution, minResolution, currResolution;
        }

        [DllImport("IOptimize.dll")]
        public static extern TimerResolutionValues IOptimizeQueryTimerResolution();

        [DllImport("IOptimize.dll")]
        public static extern UInt32 IOptimizeMicroAdjustTimerResolution(UInt32 start, UInt32 end, UInt32 increment, UInt32 samples);

    }
}
