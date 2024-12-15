using System.Diagnostics;

namespace aoc2015.days;

public class Day22 : IDay
{
    private class Effect
    {
        internal readonly int PlayerArmor;
        internal readonly int BossDamage;
        internal readonly int ManaRegen;
        internal readonly int LastsFor;

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
        internal readonly int ManaCost;
        internal readonly int Damage;
        internal readonly int Heal;
        internal readonly Effect? Effect;

        public Spell(int manaCost, int damage, int heal)
        {
            ManaCost = manaCost;
            Damage = damage;
            Heal = heal;
        }

        public Spell(int manaCost, Effect? effect)
        {
            ManaCost = manaCost;
            this.Effect = effect;
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


    public uint Day => 22;


    private readonly Spell _lstrike = new(53, 4, 0);
    private readonly Spell _drain = new(73, 2, 2);
    private readonly Spell _shield = new(113, new(7, 0, 0, 6));
    private readonly Spell _poison = new(173, new(0, 3, 0, 6));
    private readonly Spell _recharge = new(229, new(0, 0, 101, 5));

    public void Run()
    {
        List<Spell> spells =
        [
            _lstrike, _drain, _shield, _poison, _recharge,
        ];

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
    }

    private static int FindMinimumMana(GameState start, List<Spell> spells)
    {
        const int bossDamage = 9;


        var states = new Stack<(GameState, List<Spell> cast)>();
        states.Push((start, []));

        var mmtw = int.MaxValue;

        while (states.Count > 0)
        {
            var (cstate, cseq) = states.Pop();

            // pt2
            cstate.PlayerHp--;
            if (cstate.PlayerHp <= 0)
            {
                continue;
            }

            // player
            ApplyEffects(cstate);
            if (cstate.BossHp <= 0)
            {
                mmtw = Math.Min(mmtw, cstate.TotalManaSpent);
            }

            foreach (var spell in spells)
            {
                if (!CanCastSpell(cstate, spell)) continue;
                var news = cstate.Clone();
                news.PlayerMana -= spell.ManaCost;
                news.TotalManaSpent += spell.ManaCost;

                if (news.TotalManaSpent >= mmtw)
                {
                    continue;
                }

                if (spell.Effect != null)
                {
                    Trace.Assert(!news.ActiveEffects.ContainsKey(spell.Effect));
                    news.ActiveEffects[spell.Effect] = spell.Effect.LastsFor;
                    news.PlayerArmor += spell.Effect.PlayerArmor;
                    news.PlayerDamage += spell.Effect.BossDamage;
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

                news.PlayerHp -= Math.Max(1, bossDamage - news.PlayerArmor);

                if (news.PlayerHp > 0)
                {
                    var newSequence = new List<Spell>(cseq) {spell};
                    states.Push((news, newSequence));
                }
            }
        }

        return mmtw;
    }

    private static bool CanCastSpell(GameState state, Spell s)
    {
        if (s.Effect != null && state.ActiveEffects.ContainsKey(s.Effect)) return false;
        return state.PlayerMana >= s.ManaCost;
    }

    private static void ApplyEffects(GameState state)
    {
        var postRemove = new List<Effect>();

        foreach (var (effect, _) in state.ActiveEffects)
        {
            state.PlayerMana += effect.ManaRegen;
            state.BossHp -= effect.BossDamage;

            state.ActiveEffects[effect]--;
            if (state.ActiveEffects[effect] == 0)
            {
                postRemove.Add(effect);
                state.PlayerArmor -= effect.PlayerArmor;
                state.PlayerDamage -= effect.BossDamage;
            }
        }


        foreach (var effect in postRemove)
        {
            state.ActiveEffects.Remove(effect);
        }
    }
}
