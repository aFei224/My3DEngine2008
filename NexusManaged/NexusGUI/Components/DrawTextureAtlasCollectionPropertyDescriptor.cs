﻿using System;
using System.ComponentModel;

namespace Nexus.GUI.Components
{
    /// <summary>
    /// Summary description for CollectionPropertyDescriptor.
    /// </summary>
    public class DrawTextureAtlasCollectionPropertyDescriptor : PropertyDescriptor
    {
        private DrawTextureAtlasCollection collection;
        private int index = -1;

        public DrawTextureAtlasCollectionPropertyDescriptor(DrawTextureAtlasCollection coll, int idx) :
            base("#" + idx.ToString(System.Globalization.CultureInfo.CurrentCulture), null)
        {
            this.collection = coll;
            this.index = idx;
        }

        public override AttributeCollection Attributes
        {
            get
            {
                return new AttributeCollection(null);
            }
        }

        public override bool CanResetValue(object component)
        {
            return true;
        }

        public override Type ComponentType
        {
            get
            {
                return this.collection.GetType();
            }
        }

        public override string DisplayName
        {
            get
            {
                UIComp_DrawTextureAtlas item = this.collection[index];
                return item.ResourceAtlasLocation.ToString();
            }
        }

        public override string Description
        {
            get
            {
                UIComp_DrawTextureAtlas item = this.collection[index];
                return item.ResourceAtlasLocation.ToString();
            }
        }

        public override object GetValue(object component)
        {
            return this.collection[index];
        }

        public override bool IsReadOnly
        {
            get { return false; }
        }

        public override string Name
        {
            get { return "#" + index.ToString(System.Globalization.CultureInfo.CurrentCulture); }
        }

        public override Type PropertyType
        {
            get { return this.collection[index].GetType(); }
        }

        public override void ResetValue(object component)
        {
        }

        public override bool ShouldSerializeValue(object component)
        {
            return true;
        }

        public override void SetValue(object component, object value)
        {
            // this.collection[index] = value;
        }
    }
}
