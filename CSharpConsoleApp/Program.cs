/////////////////////////////////////////////////////////////////////////////////////////
// C# application passing custom struct to a C++ native DLL
// by Giovanni Dicanio
/////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Runtime.InteropServices;

namespace CSharpConsoleApp
{
    /// <summary>
    /// This is a custom data structure that maps to the following C++ struct:
    ///
    ///    struct MyCppDll_Data
    ///    {
    ///        GUID Id;
    ///        int Value;
    ///        const wchar_t* Name;
    ///    };
    ///
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Unicode)]
    public struct DllData
    {
        public Guid Id;
        public Int32 Value;
        public String Name;
    }

    class Program
    {
        /// <summary>
        /// DllImport for the following C-interface function exported by the C++ DLL:
        ///
        /// extern "C" HRESULT __stdcall MyCppDll_ProcessData(const MyCppDll_Data* pData)
        ///
        /// NOTE: The PreserveSig=false converts error HRESULT return codes to C# exceptions.
        /// </summary>
        ///
        /// <param name="data">Custom data structure to be passed to the native C++ DLL</param>
        [DllImport("MyCppDll.dll", EntryPoint="MyCppDll_ProcessData",
            CallingConvention = CallingConvention.StdCall,
            ExactSpelling = true,
            PreserveSig = false)]
        static extern void ProcessData([In] ref DllData data);

        static void Main(string[] args)
        {
            // Create an instance of the custom struct here in C#
            DllData data = new DllData { Id = Guid.NewGuid(), Value = 10, Name = "Connie" };

            // Print out its data members
            Console.WriteLine("C# Data:");
            Console.WriteLine("Id = {0}", data.Id);
            Console.WriteLine("Value = {0}", data.Value);
            Console.WriteLine("Name = {0}", data.Name);

            // Pass it to the C++ DLL
            Console.WriteLine("Invoking the C++ DLL...");
            ProcessData(ref data);
        }
    }
}
