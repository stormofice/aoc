namespace aoc2015.days;

public class Day23: IDay
{
    public uint Day => 23;
    public void Run()
    {
        var insns = Utils.GetDayInput(this).Split("\n");


        var pc = 0;

        var regs = new Dictionary<string, ulong>
        {
            ["a"] = 1,
            ["b"] = 0,
        };

        while (true)
        {
            if (pc >= insns.Length)
                break;

            var c = insns[pc];

            var s = c.Split(" ", 2);
            switch (s[0])
            {
                case "hlf":
                    regs[s[1]] /= 2;
                    pc++;
                    break;
                case "tpl":
                    regs[s[1]] *= 3;
                    pc++;
                    break;
                case "inc":
                    regs[s[1]]++;
                    pc++;
                    break;
                case "jmp":
                    pc += (int.Parse(s[1]));
                    break;
                case "jie":
                    var cs = s[1].Split(",");
                    if (regs[cs[0]] % 2 == 0)
                        pc += int.Parse(cs[1]);
                    else
                        pc++;

                    break;
                case "jio":
                    var ccs = s[1].Split(",");
                    if (regs[ccs[0]]== 1)
                        pc += int.Parse(ccs[1]);
                    else
                        pc++;

                    break;
                default:
                    throw new ArgumentException();
            }

        }

        foreach (var (r, v) in regs)
        {
            Console.WriteLine($"{r}: {v}");
        }

    }
}
