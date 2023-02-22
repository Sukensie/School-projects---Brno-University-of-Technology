using System.Windows;

namespace ICSProject.App.Controls
{
    public partial class SubHeading
    {
        public string Txt
        {
            get => (string)GetValue(TxtProperty);
            set => SetValue(TxtProperty, value); 
        }

        public static readonly DependencyProperty TxtProperty = DependencyProperty.Register(
            nameof(Txt),
            typeof(string),
            typeof(SubHeading),
            new PropertyMetadata(default(string)));

        public SubHeading()
        {
            InitializeComponent();
        }
    }
}
