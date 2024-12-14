using System.Security.Cryptography;

namespace aoc2015.days;

public class Day04: IDay
{
    public uint Day => 4;
    public void Run()
    {
        var inp = Utils.GetDayInput(this);
        // var inp = "abcdef";

        for (var i = 0; ; i++)
        {
            var tryy = $"{inp}{i}";
            var hash = MD5.HashData(System.Text.Encoding.UTF8.GetBytes(tryy));
            var hshstr = Convert.ToHexString(hash);

            if (hshstr.StartsWith("000000"))
            {
                Console.WriteLine($"{i}: {hshstr}");
                break;
            }
        }

    }
}
