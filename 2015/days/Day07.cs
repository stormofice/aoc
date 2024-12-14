using System.Diagnostics;

namespace aoc2015.days;


public class Day07: IDay
{
    private enum Operation
    {
        And,
        Or,
        LShift,
        RShift,
        Not,
        Immediate
    }

    private record class Command(Operation Op, string Result, List<string> Ins);


    public uint Day => 7;

    private ushort GetValue(Dictionary<string, ushort> vars, Dictionary<string, Command> cmds, string v)
    {
        var (op, res, ins) = cmds[v];

        Console.WriteLine(v);

        var get = (int i) =>
        {
            Debug.Assert(i >= 1 & i <= 2);
            var target = i == 1 ? ins[0] : ins[1];
            if (ushort.TryParse(target, out var fop))
                return fop;
            else
            {
                if (vars.TryGetValue(target, out var vvv))
                {
                    return vvv;
                }
                var val = GetValue(vars, cmds, target);
                vars[target] = val;
                return val;
            }
        };

        return op switch
        {
            Operation.And => (ushort) (get(1) & get(2)),
            Operation.Or => (ushort) (get(1) | get(2)),
            Operation.LShift => (ushort) (get(1) << get(2)),
            Operation.RShift => (ushort) (get(1) >> get(2)),
            Operation.Not => (ushort) ~get(1),
            Operation.Immediate => get(1),
            _ => throw new ArgumentOutOfRangeException()
        };
    }

    public void Run()
    {
        // var inp = Utils.GetDaySampleInputs(this).First().Split("\n");
        var inp = Utils.GetDayInput(this).Split("\n");

        var vars = new Dictionary<string, ushort>();
        // Map result variable to command for faster tree construction
        var cmds = new Dictionary<string, Command>();

        foreach (var line in inp)
        {
            var ts = line.Split("->").Select(s => s.Trim()).ToList();
            Debug.Assert(ts.Count == 2);
            var left = ts[0];
            var right = ts[1];

            Operation op;
            var ins = new List<string>();

            if (left.Contains("AND"))
            {
                op = Operation.And;
                ins = left.Split("AND").Select(s => s.Trim()).ToList();
            } else if (left.Contains("OR"))
            {
                op = Operation.Or;
                ins = left.Split("OR").Select(s => s.Trim()).ToList();
            } else if (left.Contains("LSHIFT"))
            {
                op = Operation.LShift;
                ins = left.Split("LSHIFT").Select(s => s.Trim()).ToList();
            }
            else if (left.Contains("RSHIFT"))
            {
                op = Operation.RShift;
                ins = left.Split("RSHIFT").Select(s => s.Trim()).ToList();

            } else if (left.Contains("NOT"))
            {
                op = Operation.Not;
                ins = [left.Replace("NOT", string.Empty).Trim()];
            }
            else
            {
                op = Operation.Immediate;
                ins = [left];
            }

            cmds.Add(right, new(op, right, ins));
            Console.WriteLine($"{cmds.Last()}");
        }


        foreach (var (var, val) in vars)
        {
            Console.WriteLine($"{var}: {val}");
        }

        Console.WriteLine(GetValue(vars,cmds, "a"));
        vars.Clear();
        vars["b"] = 3176;
        Console.WriteLine(GetValue(vars, cmds, "a"));


    }

    /* Too simple emulation code
     *
       foreach (var (operation, result, ins) in cmds)
       {

           try
           {

               var get = (int i) =>
               {
                   Debug.Assert(i >= 1 & i <= 2);
                   var target = i == 1 ? ins[0] : ins[1];

                   return ushort.TryParse(target, out var fop) ? fop : vars[target];
               };

               vars[result] = operation switch
               {
                   Operation.And => (ushort) (get(1) & get(2)),
                   Operation.Or => (ushort) (get(1) | get(2)),
                   Operation.LShift => (ushort) (get(1) << get(2)),
                   Operation.RShift => (ushort) (get(1) >> get(2)),
                   Operation.Not => (ushort) ~(get(1)),
                   Operation.Immediate => ushort.Parse(ins[0]),
                   _ => throw new ArgumentOutOfRangeException()
               };

           }
           catch
           {
               Console.WriteLine($"Skipping {operation}");
           }
       }
     */
}
