using System.Diagnostics;

namespace aoc2015.days;

public class Day99 : IDay
{
    private class Player
    {
        internal int Damage;
        internal int Armor;
        internal int Hitpoints;
        internal int Mana;

        internal void Reset()
        {
            Damage = 0;
            Armor = 0;
            Hitpoints = 50;
            Mana = 500;
        }

        public override string ToString()
        {
            return
                $"{nameof(Damage)}: {Damage}, {nameof(Armor)}: {Armor}, {nameof(Hitpoints)}: {Hitpoints}, {nameof(Mana)}: {Mana}";
        }
    }

    private class Boss
    {
        internal int Hitpoints = 51;
        internal int Damage = 9;

        internal void Reset()
        {
            Hitpoints = 51;
        }

        public override string ToString()
        {
            return $"{nameof(Hitpoints)}: {Hitpoints}, {nameof(Damage)}: {Damage}";
        }
    }

    private class Effect
    {
        internal int PlayerArmor;
        internal int BossDamage;
        internal int ManaRegen;
        internal int LastsFor;

        public Effect(int playerArmor, int bossDamage, int manaRegen, int lastsFor)
        {
            PlayerArmor = playerArmor;
            BossDamage = bossDamage;
            ManaRegen = manaRegen;
            LastsFor = lastsFor;
        }
    }

    private class Spell
    {
        internal int ManaCost;
        internal int Damage;
        internal int Heal;
        internal Effect? effect;

        public Spell(int manaCost, int damage, int heal)
        {
            ManaCost = manaCost;
            Damage = damage;
            Heal = heal;
        }

        public Spell(int manaCost, Effect? effect)
        {
            ManaCost = manaCost;
            this.effect = effect;
        }

        protected bool Equals(Spell other)
        {
            return ManaCost == other.ManaCost && Damage == other.Damage && Heal == other.Heal &&
                   Equals(effect, other.effect);
        }

        public override bool Equals(object? obj)
        {
            if (obj is null) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != GetType()) return false;
            return Equals((Spell) obj);
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(ManaCost, Damage, Heal, effect);
        }

        public static bool operator ==(Spell? left, Spell? right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(Spell? left, Spell? right)
        {
            return !Equals(left, right);
        }

        public override string ToString()
        {
            switch (ManaCost)
            {
                case 53:
                    return "strike";
                case 73:
                    return "drain";
                case 113:
                    return "shield";
                case 173:
                    return "poison";
                case 229:
                    return "recharge";
                default:
                    Trace.Assert(false);
                    break;
            }

            return "";
        }
    }


    private class GameState
    {
        internal int PlayerHp;
        internal int PlayerMana;
        internal int PlayerArmor;
        internal int PlayerDamage;
        internal int BossHp;
        internal Dictionary<Effect, int> ActiveEffects = new();
        internal int TotalManaSpent;

        public GameState Clone()
        {
            return new()
            {
                PlayerHp = PlayerHp,
                PlayerMana = PlayerMana,
                PlayerArmor = PlayerArmor,
                PlayerDamage = PlayerDamage,
                BossHp = BossHp,
                ActiveEffects = new(ActiveEffects),
                TotalManaSpent = TotalManaSpent,
            };
        }
    }

    private static IEnumerable<List<Spell>> GenerateSpellPermutations(List<Spell> allSpells, int length)
    {
        var currentSeq = new int[length];

        while (true)
        {
            var spells = new List<Spell>(length);
            for (var i = 0; i < length; i++)
            {
                spells.Add(allSpells[currentSeq[i]]);
            }

            yield return spells;

            var pos = length - 1;
            while (pos >= 0)
            {
                currentSeq[pos]++;
                if (currentSeq[pos] == allSpells.Count)
                {
                    currentSeq[pos] = 0;
                    pos--;
                }
                else
                {
                    break;
                }
            }

            if (pos < 0)
            {
                break;
            }
        }
    }

    public uint Day => 22;

    private int ToId(List<Spell> spells)
    {
        var id = 0;
        var idx = 0;
        foreach (var spell in spells)
        {
            if (spell == drain)
                id += (int) Math.Pow(10, idx) * 1;
            if (spell == shield)
                id += (int) Math.Pow(10, idx) * 2;
            if (spell == poison)
                id += (int) Math.Pow(10, idx) * 3;
            if (spell == recharge)
                id += (int) Math.Pow(10, idx) * 4;
            idx++;
        }

        return id;
    }

    Spell lstrike = new(53, 4, 0);
    Spell drain = new(73, 2, 2);
    Spell shield = new(113, new(7, 0, 0, 6));
    Spell poison = new(173, new(0, 3, 0, 6));
    Spell recharge = new(229, new(0, 0, 101, 5));

    public void Run()
    {
        List<Spell> spells =
        [
            lstrike, drain, shield, poison, recharge,
        ];

        // player.Mana = 250;
        // player.Hitpoints = 10;
        // boss.Hitpoints = 14;
        // boss.Damage = 8;

        // List<Spell> ts = [new(173, new(0, 3, 0, 6)), new(53, 4, 0)];
        // List<Spell> ts = [recharge,shield,drain,poison,lstrike];

        var start = new GameState
        {
            PlayerHp = 50,
            PlayerMana = 500,
            BossHp = 51,
            PlayerArmor = 0,
            TotalManaSpent = 0,
            PlayerDamage = 0,
        };

        Console.WriteLine($"MMTW: {FindMinimumMana(start, spells)}");

        /*
        var player = new Player();
        var boss = new Boss();

        var minManaCost = int.MaxValue;
        const int length = 12;

        var noOut = 0;
        foreach (var perms in GenerateSpellPermutations(spells, length))
        {
            var res = Battle(player, boss, perms);

            if (res is {Item1: true, Item3: true})
            {
                minManaCost = Math.Min(minManaCost, res.Item2);
                Console.WriteLine($"min: {minManaCost}");
            }

            if (!res.Item3)
            {
                noOut++;
            }
        }

        Console.WriteLine($"Min Mana: {minManaCost}, No outcome: {noOut}");*/
    }

    private int FindMinimumMana(GameState start, List<Spell> spells)
    {
        const int bossDamage = 9;


        var states = new Stack<(GameState, List<Spell> cast)>();
        states.Push((start, []));

        var mmtw = int.MaxValue;

        while (states.Count > 0)
        {
            var (cstate, cseq) = states.Pop();

            // player
            ApplyEffects(cstate);
            if (cstate.BossHp <= 0)
            {
                mmtw = Math.Min(mmtw, cstate.TotalManaSpent);
            }

            foreach (var spell in spells)
            {
                if (CanCastSpell(cstate, spell))
                {
                    var news = cstate.Clone();
                    news.PlayerMana -= spell.ManaCost;
                    news.TotalManaSpent += spell.ManaCost;

                    if (news.TotalManaSpent >= mmtw)
                    {
                        continue;
                    }

                    if (spell.effect != null)
                    {
                        Trace.Assert(!news.ActiveEffects.ContainsKey(spell.effect));
                        news.ActiveEffects[spell.effect] = spell.effect.LastsFor;
                        news.PlayerArmor += spell.effect.PlayerArmor;
                        news.PlayerDamage += spell.effect.BossDamage;
                    }
                    else
                    {
                        news.BossHp -= spell.Damage;
                        news.PlayerHp += spell.Heal;
                    }

                    if (news.BossHp <= 0)
                    {
                        mmtw = Math.Min(mmtw, news.TotalManaSpent);
                        continue;
                    }

                    ApplyEffects(news);

                    if (news.BossHp <= 0)
                    {
                        mmtw = Math.Min(mmtw, news.TotalManaSpent);
                        continue;
                    }

                    Trace.Assert((bossDamage - news.PlayerArmor) > 1);
                    news.PlayerHp -= Math.Max(1, bossDamage - news.PlayerArmor);

                    if (news.PlayerHp > 0)
                    {
                        var newSequence = new List<Spell>(cseq) {spell};
                        states.Push((news, newSequence));
                    }
                }
            }
        }

        return mmtw;
    }


    private (bool, int, bool, int) Battle(Player p, Boss b, List<Spell> turnSpells)
    {
        var manaCost = 0;
        p.Reset();
        b.Reset();

        var playerWin = false;

        Dictionary<Effect, int> active = [];

        var idx = 0;

        while (idx < turnSpells.Count)
        {
            // player turn
            ApplyEffects(p, b, active);
            if (b.Hitpoints <= 0)
            {
                playerWin = true;
                break;
            }


            var spell = turnSpells[idx];

            if (p.Mana < spell.ManaCost)
            {
                // Instant lose, no mana
                break;
            }

            manaCost += spell.ManaCost;
            p.Mana -= spell.ManaCost;

            if (spell.effect is { } e)
            {
                if (active.TryAdd(e, e.LastsFor))
                {
                    p.Damage += e.BossDamage;
                    p.Armor += e.PlayerArmor;
                }
            }
            else
            {
                b.Hitpoints -= spell.Damage;
                p.Hitpoints += spell.Heal;
            }

            if (b.Hitpoints <= 0)
            {
                playerWin = true;
                break;
            }

            // boss turn
            ApplyEffects(p, b, active);
            if (b.Hitpoints <= 0)
            {
                playerWin = true;
                break;
            }


            p.Hitpoints -= (b.Damage - p.Armor);

            if (p.Hitpoints <= 0)
            {
                playerWin = false;
                break;
            }

            idx++;
        }

        return (playerWin, manaCost, p.Hitpoints <= 0 || b.Hitpoints <= 0, idx);
    }

    private static bool CanCastSpell(GameState state, Spell s)
    {
        if (s.effect != null && state.ActiveEffects.ContainsKey(s.effect)) return false;
        return state.PlayerMana >= s.ManaCost;
    }

    private static void ApplyEffects(GameState state)
    {
        var postRemove = new List<Effect>();

        foreach (var (effect, _) in state.ActiveEffects)
        {
            state.PlayerMana += effect.ManaRegen;

            state.ActiveEffects[effect]--;
            if (state.ActiveEffects[effect] == 0)
            {
                postRemove.Add(effect);
                state.PlayerArmor -= effect.PlayerArmor;
                state.PlayerDamage -= effect.BossDamage;
            }
        }

        state.BossHp -= state.PlayerDamage;

        foreach (var effect in postRemove)
        {
            state.ActiveEffects.Remove(effect);
        }
    }

    private static void ApplyEffects(Player p, Boss b, Dictionary<Effect, int> active)
    {
        foreach (var (key, _) in active)
        {
            p.Mana += key.ManaRegen;
            active[key]--;
            if (active[key] == 0)
            {
                p.Damage -= key.BossDamage;
                p.Armor -= key.PlayerArmor;
                active.Remove(key);
            }
        }

        b.Hitpoints -= p.Damage;
    }
}
