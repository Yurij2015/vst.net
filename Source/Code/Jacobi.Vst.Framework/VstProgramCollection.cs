﻿namespace Jacobi.Vst.Framework
{
    using Jacobi.Vst.Core;
    using Jacobi.Vst.Framework.Common;
    using System.Collections.Generic;

    /// <summary>
    /// Manages a collection of <see cref="VstProgram"/> instances.
    /// </summary>
    public class VstProgramCollection : ObservableCollection<VstProgram>
    {
        /// <summary>
        /// Adds a range of <see cref="VstProgram"/> instances to the collection.
        /// </summary>
        /// <param name="programs">Must not be null.</param>
        public void AddRange(VstProgramCollection programs)
        {
            Throw.IfArgumentIsNull(programs, "programs");

            foreach (VstProgram program in programs)
            {
                Add(program);
            }
        }

        /// <summary>
        /// Overridden to <see cref="VstProgram.Dispose"/> the instances.
        /// </summary>
        protected override void ClearItems()
        {
            List<VstProgram> programs = new List<VstProgram>(this);

            base.ClearItems();

            foreach (VstProgram program in programs)
            {
                program.Dispose();
            }
        }

        /// <summary>
        /// Overridden to <see cref="VstProgram.Dispose"/> the removed instance.
        /// </summary>
        /// <param name="index">A zero-based index that will be removed.</param>
        protected override void RemoveItem(int index)
        {
            VstProgram program = this[index];

            base.RemoveItem(index);

            program.Dispose();
        }

        /// <summary>
        /// Overridden to <see cref="VstProgram.Dispose"/> the replaced instance.
        /// </summary>
        /// <param name="index">A zero-based index the new <paramref name="item"/> will be set.</param>
        /// <param name="item">The new item to set. Can be null.</param>
        protected override void SetItem(int index, VstProgram item)
        {
            VstProgram program = this[index];

            base.SetItem(index, item);

            if (program != item)
            {
                program.Dispose();
            }
        }
    }
}
