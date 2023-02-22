using System.Windows;
using System.Windows.Input;

namespace ICSProject.App.Controls
{
    public partial class PrizeTag
    {
        public string Txt
        {
            get => (string)GetValue(TxtProperty);
            set => SetValue(TxtProperty, value); 
        }


        public static readonly DependencyProperty TxtProperty = DependencyProperty.Register(
            nameof(Txt),
            typeof(string),
            typeof(PrizeTag),
            new PropertyMetadata(default(string)));

        public PrizeTag()
        {
            InitializeComponent();
        }
    }
}
