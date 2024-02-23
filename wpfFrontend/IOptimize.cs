using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

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

        [DllImport("IOptimize.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType);

        // Parameter msi is to be dealt with like a boolean
        [DllImport("IOptimize.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IOptimizeSetGpuMsiMode(IOptimizeBool msi);

        [DllImport("IOptimize.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IOptimizeSetDisplayResolution(int width, int height);

        [DllImport("IOptimize.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IOptimizeSetTimerResolution(UInt32 resolutionMs);

        readonly public struct TimerResolutionValues
        {
            public UInt32 maxResolution  { get; }
            public UInt32 minResolution  { get; }
            public UInt32 currResolution { get; }
        }

        [DllImport("IOptimize.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern TimerResolutionValues IOptimizeQueryTimerResolution();

        [DllImport("IOptimize.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 IOptimizeMicroAdjustTimerResolution(UInt32 start, UInt32 end, UInt32 increment, UInt32 samples);
        
        public struct IOptimizePowerOption
        {
            // Make sure name is a C wchar_t*
            [MarshalAs(UnmanagedType.LPWStr)]
            public string name;

            public UInt32 value { get; set; }
            public UInt32 maxValue { get; set; }
        }

        [DllImport("IOptimize.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IOptimizeSetPowerScheme(IOptimizePowerOption[] powerOptions, UInt64 powerOptionsSize, int subPowerScheme);

    }
}
